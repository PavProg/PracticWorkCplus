#ifndef TEXTURE_LOADER_HPP
#define TEXTURE_LOADER_HPP

#include <memory>
#include <string>

#include "Resource.hpp"
#include "Texture.hpp"

class IRenderAdapter;

namespace TextureLoader {
	std::shared_ptr<Resource<Texture>> Load(const std::string& path, IRenderAdapter& renderer);
}

#endif