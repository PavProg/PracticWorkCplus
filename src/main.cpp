#include "application/application.hpp"
#include "ecs/World.hpp"
#include "ecs/components/Transform.hpp"
#include "ecs/components/Tag.hpp"
#include "ecs/components/MeshRenderer.hpp"
#include <glm/glm.hpp>
#include "logger/logger.hpp"

struct TestComponent {
    int value;
};

void TestComponents() {
    Logger::Info("=== Components Test Started ===");

    World world;

    EntityId player = world.CreateEntity();
    EntityId enemy = world.CreateEntity();
    EntityId powerUp = world.CreateEntity();

    Logger::Info("Created entities: " + std::to_string(player) + ", " +
                 std::to_string(enemy) + ", " + std::to_string(powerUp));

    world.AddComponent<Tag>(player, {"Player"});
    world.AddComponent<Transform>(player, Transform{});
    world.AddComponent<MeshRenderer>(player, {PrimitiveType::Square, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)});

    Transform enemyTransform;
    enemyTransform.position = glm::vec3(2.0f, 0.0f, 0.0f);
    world.AddComponent<Tag>(enemy, {"Enemy"});
    world.AddComponent<Transform>(enemy, enemyTransform);
    world.AddComponent<MeshRenderer>(enemy, {PrimitiveType::Triangle, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)});

    world.AddComponent<Tag>(powerUp, {"PowerUp"});
    world.AddComponent<MeshRenderer>(powerUp, {PrimitiveType::Square, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)});

    auto PrintEntityInfo = [&](EntityId id) {
        Logger::Info("Entity " + std::to_string(id) + ":");
        if (world.HasComponent<Tag>(id)) {
            auto& tag = world.GetComponent<Tag>(id);
            Logger::Info("  Tag: " + tag.name);
        }
        if (world.HasComponent<Transform>(id)) {
            auto& tr = world.GetComponent<Transform>(id);
            Logger::Info("  Transform: pos(" + std::to_string(tr.position.x) + ", " +
                         std::to_string(tr.position.y) + ", " + std::to_string(tr.position.z) + ")");
        }
        if (world.HasComponent<MeshRenderer>(id)) {
            auto& mr = world.GetComponent<MeshRenderer>(id);
            std::string primType;
            switch (mr.primitiveType) {
            case PrimitiveType::Triangle: primType = "Triangle"; break;
            case PrimitiveType::Square: primType = "Square"; break;
            case PrimitiveType::Cube: primType = "Cube"; break;
            }
            Logger::Info("  MeshRenderer: type=" + primType + ", color=(" +
                         std::to_string(mr.color.r) + ", " +
                         std::to_string(mr.color.g) + ", " +
                         std::to_string(mr.color.b) + ", " +
                         std::to_string(mr.color.a) + ")");
        }
        Logger::Info("  -----");
    };

    PrintEntityInfo(player);
    PrintEntityInfo(enemy);
    PrintEntityInfo(powerUp);

    if (world.HasComponent<Transform>(player)) {
        auto& tr = world.GetComponent<Transform>(player);
        tr.position.x = 5.0f;
        Logger::Info("Updated player position to: " + std::to_string(tr.position.x));
    }

    const auto& meshRenderers = world.GetAllComponents<MeshRenderer>();
    Logger::Info("Total entities with MeshRenderer: " + std::to_string(meshRenderers.size()));

    Logger::Info("=== Components Test Finished ===");
}

int main() {
    Application app;
    if (!app.Init(1000, 800, "My game Engine")) {
        return -1;
    }

    TestComponents();

    app.Run();
    return 0;
}