#ifndef SHADER_LOADER_HPP
#define SHADER_LOADER_HPP

#include <memory>
#include <string>

#include "Resource.hpp"
#include "ShaderProgram.hpp"

class IRenderAdapter;

namespace ShaderLoader {
	std::shared_ptr<Resource<ShaderProgram>> Load(const std::string& basePath, IRenderAdapter& renderer);
}

#endif