#include "RenderSystem.hpp"
#include "ecs/components/MeshRenderer.hpp"
#include "ecs/components/Transform.hpp"
#include "logger/logger.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "ecs/components/Hierarchy.hpp"

static constexpr int MAX_HIERARCHY_DEPTH = 32;  // Пока максимальный уровень глубины рекурсии 32 должно быть достаточно

RenderSystem::RenderSystem(World& world, IRenderAdapter& renderer) : m_world(world), m_renderer(renderer) {}

// Локальная матрица объекта
static glm::mat4 BuildLocalMatrix(const Transform& t) {
    glm::mat4 m = glm::mat4(1.0f);
    m = glm::translate(m, t.position);
    m = m * glm::mat4_cast(t.rotation);
    m = glm::scale(m, t.scale);
    return m;
}

// Рекурсивная функция для вычисления локальной матрицы. Иерархия реализуется следующим образом
// мировая-матрица-ребенка = мировая-матрица-родителя * локальная-матрица-ребенка
glm::mat4 RenderSystem::ComputeWorldMatrix(EntityId entity, int depth) {
    if (depth > MAX_HIERARCHY_DEPTH) {
        Logger::Warning("Hierarchy too deеp (possible cycle) at entity " + std::to_string(entity));
        return glm::mat4(1.0f);
    }

    if (!m_world.HasComponent<Transform>(entity)) {
        return glm::mat4(1.0f);
    }

    const Transform& t = m_world.GetComponent<Transform>(entity);
    glm::mat4 localMatrix = BuildLocalMatrix(t);

    if (!m_world.HasComponent<Hierarchy>(entity)) {
        return localMatrix;
    }
    const Hierarchy& h = m_world.GetComponent<Hierarchy>(entity);
    if (h.parent == INVALID_ID) {
        return localMatrix;
    }

    glm::mat4 parentWorld = ComputeWorldMatrix(h.parent, depth + 1);
    return parentWorld * localMatrix;
}

void RenderSystem::Update() {
    const auto& meshRenderers = m_world.GetAllComponents<MeshRenderer>();

    for (const auto& [entity, mesh] : meshRenderers) {
        if (!m_world.HasComponent<Transform>(entity)) {
            Logger::Warning("Entity " + std::to_string(entity) +
                            " has MeshRenderer but no Transform, skipping");
            continue;
        }

        // Если у сущности нет родителя - ф-ция вернет локальную матрицу
        glm::mat4 worldMatrix = ComputeWorldMatrix(entity);

        m_renderer.DrawMesh(worldMatrix, mesh.primitiveType, mesh.color);
    }
}