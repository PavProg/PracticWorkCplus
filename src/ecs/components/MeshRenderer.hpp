#ifndef MESHRENDERER_HPP
#define MESHRENDERER_HPP

#include <glm/glm.hpp>
#include <memory>

#include "resources/Resource.hpp"
#include "resources/Mesh.hpp"
#include "resources/Texture.hpp"
#include "resources/ShaderProgram.hpp"

// None - не использовать примитив, рисовать через ресурсы
enum class PrimitiveType {
	Triangle,
	Square,
	Cube,
	None
};

struct MeshRenderer {
	PrimitiveType primitiveType = PrimitiveType::Triangle;
	glm::vec4 color = glm::vec4(1.0f);

	std::shared_ptr<Resource<Mesh>> mesh;
	std::shared_ptr<Resource<Texture>> texture;
	std::shared_ptr<Resource<ShaderProgram>> shader;
	glm::vec4 tint = glm::vec4(1.0f);
};

#endif