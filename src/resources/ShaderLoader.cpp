#include "ShaderLoader.hpp"
#include "ResourceManager.hpp"
#include "render/IRenderAdapter.hpp"
#include "logger/logger.hpp"

#include <filesystem>
#include <sstream>
#include <fstream>

namespace {
	std::string ReadFile(const std::string& path) {
		std::ifstream f(path);
		if (!f.is_open()) return {};
		std::stringstream ss;
		ss << f.rdbuf();
		return ss.str();
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
		raw->lastModified = std::filesystem::last_write_time(vsPath, ec);

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