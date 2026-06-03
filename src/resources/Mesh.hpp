#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include <cstdint>
#include <glm/glm.hpp>

// Одна вершина модели
struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 uv;
};

struct MeshData {
	std::vector<Vertex> vertices;
	std::vector<std::uint32_t> indices;
};

struct GPUMesh {
	unsigned int vao = 0;
	unsigned int vbo = 0;
	unsigned int ebo = 0; // Буффер индексов
	int indexCount = 0;
};

struct Mesh {
	MeshData cpuData;
	GPUMesh gpu;
};

#endif