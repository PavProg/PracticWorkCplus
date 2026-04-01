#ifndef MESHRENDERER_HPP
#define MESHRENDERER_HPP

#include <glm/glm.hpp>

enum class PrimitiveType {
    Triangle,
    Square,
    Cube
};

struct MeshRenderer {
    PrimitiveType primitiveType = PrimitiveType::Triangle;
    glm::vec4 color = glm::vec4(1.0f);
    // unsigned int shaderProgram;
};

#endif