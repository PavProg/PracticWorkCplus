#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <vector>
#include <cstdint>

enum class TextureFilter {
	Nearest,
	Linear
};

enum class TextureWrap {
	Repeat,
	ClampToEdge
};

struct TextureData {
	int width = 0;
	int height = 0;
	int channels = 0;
	std::vector<std::uint8_t> pixels;

	TextureFilter filter = TextureFilter::Nearest;
	TextureWrap wrap = TextureWrap::Repeat;
};

struct GPUTexture {
	unsigned int textureId = 0;
};

struct Texture {
	TextureData cpuData;
	GPUTexture gpu;
};

#endif