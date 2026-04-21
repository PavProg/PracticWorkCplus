#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <memory>
#include "render/IRenderAdapter.hpp"
#include "ecs/systems/RenderSystem.hpp"
#include "ecs/systems/AnimationSystem.hpp"
#include "scene/SceneSettings.hpp"

struct GLFWwindow;
class StateManager;

class Application {
    public:
        Application();
        ~Application();

        bool Init(int width, int height, const char* title);
        void Run();
        void Shutdown();

    private:
        GLFWwindow* window;
        bool running;
        double lastFrameTime;
        std::unique_ptr<StateManager> stateManager;
        std::unique_ptr<IRenderAdapter> renderer;
        std::unique_ptr<World> world;
        std::unique_ptr<RenderSystem> renderSystem;
        std::unique_ptr<AnimationSystem> animationSystem;

        SceneSettings m_sceneSettings;
};

#endif