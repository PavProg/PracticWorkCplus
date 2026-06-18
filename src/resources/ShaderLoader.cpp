#include "ShaderLoader.hpp"
#include "ResourceManager.hpp"
#include "render/IRenderAdapter.hpp"
#include "logger/logger.hpp"

#include <filesystem>
#include <sstream>
#include <fstream>

#include "stb/stb_image.h"

namespace {
	std::string ReadFile(const std::string& path) {
		std::ifstream f(path);
		if (!f.is_open()) return {};
		std::stringstream ss;
		ss << f.rdbuf();
		return ss.str();
	}

	std::filesystem::file_time_type LatestWriteTime(const std::string& basePath) {
		std::error_code ec;
		auto tVert = std::filesystem::last_write_time(basePath + ".vert", ec);
		auto tFrag = std::filesystem::last_write_time(basePath + ".frag", ec);
		return (tFrag > tVert) ? tFrag : tVert;
	}
}

namespace ShaderLoader {
	std::shared_ptr<Resource<ShaderProgram>> Load(const std::string& basePath, IRenderAdapter& renderer) {
		std::string vsPath = basePath + ".vert";
		std::string fsPath = basePath + ".frag";

		std::string vs = ReadFile(vsPath);
		std::string fs = ReadFile(fsPath);
		if (vs.empty() || fs.empty()) {
			Logger::Error("ShaderLoader: cannot read " + vsPath + " or " + fsPath);
			return nullptr;
		}

		GPUShader gpu = renderer.CompileShader(vs, fs);
		if (gpu.programId == 0) {
			Logger::Error("ShaderLoader: compilation failed for " + basePath);
			return nullptr;
		}

		Resource<ShaderProgram>* raw = new Resource<ShaderProgram>{};
		raw->path = basePath;
		raw->data.sources.fragment = std::move(fs);
		raw->data.sources.vertex = std::move(vs);
		raw->data.gpu = gpu;

		std::error_code ec;
		raw->lastModified = LatestWriteTime(basePath);

		auto deleter = [&renderer](Resource<ShaderProgram>* p) {
			renderer.ReleaseShader(p->data.gpu);
			delete p;
		};

		Logger::Info("ShaderLoader: compiled " + basePath);
		return std::shared_ptr<Resource<ShaderProgram>>(raw, deleter);
	}
}

template<>
std::shared_ptr<Resource<ShaderProgram>>
ResourceManager::LoadImpl<ShaderProgram>(const std::string& path) {
	return ShaderLoader::Load(path, m_renderer);
}

void ResourceManager::ReloadModifiedShaders() {
	auto typeIt = m_caches.find(std::type_index(typeid(ShaderProgram)));
	if (typeIt == m_caches.end()) return;

	for (auto& [path, weak] : typeIt->second) {
		std::shared_ptr<void> alive = weak.lock();
		if (!alive) continue;
		auto res = std::static_pointer_cast<Resource<ShaderProgram>>(alive);

		std::filesystem::file_time_type current = LatestWriteTime(res->path);
		if (current <= res->lastModified) continue;

		Logger::Info("HotReload: change detected in " + res->path);

		std::string vs = ReadFile(res->path + ".vert");
		std::string fs = ReadFile(res->path + ".frag");
		if (vs.empty() || fs.empty()){
			Logger::Warning("HotReload: cannot read sources for " + res->path);
			res->lastModified = current;
			continue;
		}

		GPUShader newGpu = m_renderer.CompileShader(vs, fs);
		if (newGpu.programId == 0) {
			Logger::Error("HotReload: compile failed for " + res->path
				+ " (keeping old program)");
			res->lastModified = current;
			continue;
		}

		m_renderer.ReleaseShader(res->data.gpu);
		res->data.gpu = newGpu;
		res->data.sources.vertex = std::move(vs);
		res->data.sources.fragment = std::move(fs);
		res->lastModified = current;

		Logger::Info("HotReload: reloaded shader " + res->path);
	}
}

void ResourceManager::ReloadModifiedTextures() {
	auto typeIt = m_caches.find(std::type_index(typeid(Texture)));
	if (typeIt == m_caches.end()) return;

	for (auto& [path, weak] : typeIt->second) {
		auto alive = weak.lock();
		if (!alive) continue;
		auto res = std::static_pointer_cast<Resource<Texture>>(alive);

		std::error_code ec;
		auto current = std::filesystem::last_write_time(res->path, ec);
		if (ec || current <= res->lastModified) continue;

		stbi_set_flip_vertically_on_load(true);
		int w, h, ch;
		unsigned char* pixels = stbi_load(res->path.c_str(), &w, &h, &ch, 0);
		if (!pixels) {
			Logger::Warning("HotReload: cannot read texture " + res->path);
			res->lastModified = current;
			continue;
		}

		TextureData nd;
		nd.width = w; nd.height = h; nd.channels = ch;
		nd.pixels.assign(pixels, pixels + (w * h * ch));
		nd.filter = res->data.cpuData.filter;
		nd.wrap = res->data.cpuData.wrap;
		stbi_image_free(pixels);

		GPUTexture newGpu = m_renderer.UploadTexture(nd);

		m_renderer.ReleaseTexture(res->data.gpu);
		res->data.cpuData = std::move(nd);
		res->data.gpu = newGpu;
		res->lastModified = current;

		Logger::Info("HotReload: reloaded texture " + res->path);
	}
}