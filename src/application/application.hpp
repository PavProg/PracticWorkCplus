#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <memory>
#include "render/IRenderAdapter.hpp"
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
};

#endif