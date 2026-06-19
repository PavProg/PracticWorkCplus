#include "MeshLoader.hpp"
#include "ResourceManager.hpp"
#include "render/IRenderAdapter.hpp"
#include "logger/logger.hpp"

#include "tinyobj/tiny_obj_loader.h"

#include <filesystem>

namespace MeshLoader {
    bool ParseToData(const std::string& path, MeshData& md) {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;
        bool ok = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str());
        if (!ok) return false;
        for (const auto& shape : shapes) 
            for (const auto& idx : shape.mesh.indices) {
                Vertex v{};
                v.position = {
                    attrib.vertices[3 * idx.vertex_index + 0],
                    attrib.vertices[3 * idx.vertex_index + 1],
                    attrib.vertices[3 * idx.vertex_index + 2]
                };
                if (idx.normal_index >= 0) v.normal = {
                    attrib.normals[3 * idx.normal_index + 0],
                    attrib.normals[3 * idx.normal_index + 1],
                    attrib.normals[3 * idx.normal_index + 2]
                };
                if (idx.texcoord_index >= 0) v.uv = {
                    attrib.texcoords[2 * idx.texcoord_index + 0],
                    attrib.texcoords[2 * idx.texcoord_index + 1]
                };
                md.indices.push_back((uint32_t)md.vertices.size());
                md.vertices.push_back(v);
            }
        return true;
    }

    std::shared_ptr<Resource<Mesh>> Load(const std::string& path, IRenderAdapter& renderer) {
        MeshData md;
        if (!ParseToData(path, md)) {
            return nullptr;
        }

        Resource<Mesh>* raw = new Resource<Mesh>{};
        raw->path = path;
        raw->data.cpuData = std::move(md);
        raw->data.gpu = renderer.UploadMesh(raw->data.cpuData);

        std::error_code ec;
        raw->lastModified = std::filesystem::last_write_time(path, ec);

        auto deleter = [&renderer](Resource<Mesh>* p) {
            renderer.ReleaseMesh(p->data.gpu);
            delete p;
        };

        return std::shared_ptr<Resource<Mesh>>(raw, deleter);
    }
}

template <>
std::shared_ptr<Resource<Mesh>>
ResourceManager::LoadImpl<Mesh>(const std::string& path) {
    return MeshLoader::Load(path, m_renderer);
}