#include "RenderSystem.hpp"
#include "ecs/components/MeshRenderer.hpp"
#include "ecs/components/Transform.hpp"
#include "logger/logger.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"

RenderSystem::RenderSystem(World& world, IRenderAdapter& renderer) : m_world(world), m_renderer(renderer) {}

void RenderSystem::Update() {
    const auto& meshRenderers = m_world.GetAllComponents<MeshRenderer>();

    for (const auto& [entity, mesh] : meshRenderers) {
        if (!m_world.HasComponent<Transform>(entity)) {
            Logger::Warning("Entity " + std::to_string(entity) +
                            " has MeshRenderer but no Transform, skipping");
            continue;
        }

        const Transform& transform = m_world.GetComponent<Transform>(entity);

        // Вычисляем модельную матрицу
        glm::mat4 model = glm::mat4(1.0f);

        model = glm::translate(model, transform.position);

        // преобразуем кватернион в матрицу 3x3 (или 4x4) и умножаем текущую матрицу на неё. Порядок: translate * rotate * scale
        model = model * glm::mat4_cast(transform.rotation);
        model = glm::scale(model, transform.scale);

        m_renderer.DrawMesh(model, mesh.primitiveType, mesh.color);

        // логи
        std::string primType;
        switch (mesh.primitiveType) {
            case PrimitiveType::Triangle: primType = "Triangle"; break;
            case PrimitiveType::Square:   primType = "Square";   break;
            case PrimitiveType::Cube:     primType = "Cube";     break;
        }
        Logger::Info("RenderSystem: Entity " + std::to_string(entity) +
                     " [" + primType + "] color=(" +
                     std::to_string(mesh.color.r) + "," +
                     std::to_string(mesh.color.g) + "," +
                     std::to_string(mesh.color.b) + "," +
                     std::to_string(mesh.color.a) + ")" +
                     " pos=(" + std::to_string(transform.position.x) + "," +
                     std::to_string(transform.position.y) + "," +
                     std::to_string(transform.position.z) + ")");
    }
}