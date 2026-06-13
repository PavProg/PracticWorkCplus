#ifndef MESH_LOADER_HPP
#define MESH_LOADER_HPP

#include <memory>
#include <string>

#include "Resource.hpp"
#include "Mesh.hpp"

namespace MeshLoader {
	std::shared_ptr<Resource<Mesh>> Load(const std::string& path, IRenderAdapter& renderer);
}

#endif