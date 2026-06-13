#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include <cstdint>
#include <glm/glm.hpp>

// POD структура (Plain Old Data). CPU для hr, GPU для рендера.
struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
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