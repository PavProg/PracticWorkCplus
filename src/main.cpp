#include "application/application.hpp"
#include "ecs/World.hpp"
#include "ecs/components/Transform.hpp"
#include "ecs/components/Tag.hpp"
#include "ecs/components/MeshRenderer.hpp"
#include "ecs/systems/RenderSystem.hpp"
#include <glm/glm.hpp>
#include "logger/logger.hpp"

int main() {
    Application app;
    if (!app.Init(1000, 800, "My game Engine")) {
        return -1;
    }

    app.Run();
    return 0;
}