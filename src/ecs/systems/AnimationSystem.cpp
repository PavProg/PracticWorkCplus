#include "AnimationSystem.hpp"
#include "ecs/components/Transform.hpp"
#include "ecs/components/Tag.hpp"
#include "logger/logger.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <cmath>

AnimationSystem::AnimationSystem(World& world)
    : m_world(world), m_totalTime(0.0f) {}

void AnimationSystem::Update(float deltaTime) {
    m_totalTime += deltaTime;

    const auto& tags = m_world.GetAllComponents<Tag>();

    Logger::Info("Animation loop started");
    for (const auto& [entity, tag] : tags) {
        if (!m_world.HasComponent<Transform>(entity)) {
            continue;
        }

        Transform& transform = m_world.GetComponent<Transform>(entity);

        // Анимация сущностей (красный, синий и желтая фигуры)
        if (tag.name == "RedTriangle") {
            float angle = m_totalTime * 1.0f;
            transform.rotation = glm::angleAxis(angle, glm::vec3(0.0f, 0.0f, 1.0f));
        } else if (tag.name == "BlueTriangle") {
            float centerX = 2.0f;
            float radius = 0.5f;
            float speed = 2.0f;

            transform.position.x = centerX + radius * std::cos(m_totalTime * speed);
            transform.position.y = 0.0f + radius * std::sin(m_totalTime * speed);
        } else if (tag.name == "YellowSquare") {
            float pulse = 0.8f + 0.2f * std::sin(m_totalTime * 3.0f);
            transform.scale = glm::vec3(pulse, pulse, 1.0f);
        }   // GreenSquare - точка отсчета в центре сцены
    }
    Logger::Info("Animation loop finished");
}