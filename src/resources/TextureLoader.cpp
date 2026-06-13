#include "TextureLoader.hpp"
#include "ResourceManager.hpp"
#include "render/IRenderAdapter.hpp"
#include "logger/logger.hpp"

#include "stb/stb_image.h"

#include <filesystem>

namespace TextureLoader {
	std::shared_ptr<Resource<Texture>> Load(const std::string& path, IRenderAdapter& renderer) {
		stbi_set_flip_vertically_on_load(true);

		int w, h, ch;
		unsigned char* pixels = stbi_load(path.c_str(), &w, &h, &ch, 0);
		if (!pixels) {
			Logger::Error(std::string("TextureLoader: stbi_load failed for ")
				+ path + " (" + (stbi_failure_reason() ? stbi_failure_reason() : "?") + ")");
			return nullptr;
		}

		Resource<Texture>* raw = new Resource<Texture>{};
		raw->path = path;
		raw->data.cpuData.width = w;
		raw->data.cpuData.height = h;
		raw->data.cpuData.channels = ch;
		raw->data.cpuData.pixels.assign(pixels, pixels + (w * h * ch));
		raw->data.cpuData.filter = TextureFilter::Nearest;
		raw->data.cpuData.wrap = TextureWrap::Repeat;

		stbi_image_free(pixels);

		raw->data.gpu = renderer.UploadTexture(raw->data.cpuData);

		std::error_code ec;
		raw->lastModified = std::filesystem::last_write_time(path, ec);

		Logger::Info("TextureLoader: " + path + " ("
			+ std::to_string(w) + "x" + std::to_string(h)
			+ ", " + std::to_string(ch) + " channels)");

		auto deleter = [&renderer](Resource<Texture>* p) {
			renderer.ReleaseTexture(p->data.gpu);
			delete p;
		};

		return std::shared_ptr<Resource<Texture>>(raw, deleter);
	}
}

template<>
std::shared_ptr<Resource<Texture>>
ResourceManager::LoadImpl<Texture>(const std::string& path) {
	return TextureLoader::Load(path, m_renderer);
}