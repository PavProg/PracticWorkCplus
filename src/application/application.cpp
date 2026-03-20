#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "application.hpp"
#include "logger/logger.hpp"
#include "states/StateManager.hpp"
#include "states/MenuState.hpp"
#include "states/PlayState.hpp"
#include "states/MenuState.hpp"
#include "states/PauseState.hpp"
#include "render/OpenGLAdapter.hpp"

Application::Application() 
: window(nullptr), running(false), lastFrameTime(0.0), stateManager(nullptr) {}

Application::~Application() {
    Shutdown();
}

bool Application::Init(int width, int height, const char* title) {
    if (!glfwInit()) {
        Logger::Error("Failed to initialize GLFW");
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        Logger::Error("Failed to create GLFW window");
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGL(glfwGetProcAddress)) {
        Logger::Error("Failed to initialize GLAD");
        glfwDestroyWindow(window);
        glfwTerminate();
        return false;
    }

    running = true;
    Logger::Init("game.log");
    Logger::Info("Application initialized");

    stateManager = std::make_unique<StateManager>();
    stateManager->ChangeState(std::make_unique<MenuState>());
    Logger::Info("After ChangeState in Init");

    renderer = std::make_unique<OpenGLAdapter>(window);
    if (!renderer->Init()) {
        Logger::Error("Failed to initialize render adapter");
        return false;
    }

    return true;
}

void Application::Run() {
    Logger::Info("Entering main loop");

    while (running && !glfwWindowShouldClose(window)) {
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastFrameTime;
        lastFrameTime = currentTime;

        glfwPollEvents();

        if(stateManager) {
            stateManager->Update(deltaTime);
            stateManager->Render(*renderer);
        }

        static bool previousKeyState = false;

        bool currentKeyState = glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS;
        if (currentKeyState && !previousKeyState) {
            renderer->ReloadShaders();
        }
        previousKeyState = currentKeyState;
        
        currentKeyState = glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS;
        if (currentKeyState && !previousKeyState) {
            stateManager->ChangeState(std::make_unique<PlayState>());
        }
        previousKeyState = currentKeyState;

        currentKeyState = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
        if (currentKeyState && !previousKeyState) {
            stateManager->ChangeState(std::make_unique<MenuState>());
        }
        previousKeyState = currentKeyState;

        currentKeyState = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
        if (currentKeyState && !previousKeyState) {
            stateManager->ChangeState(std::make_unique<PauseState>());
        }
        previousKeyState = currentKeyState;

        renderer->SwapBuffers();
    }
    Logger::Info("Exited main loop");
}

void Application::Shutdown() {
    Logger::Info("Shutting down application");
    if (renderer) {
        renderer->Shutdown();
        renderer.reset();
    }
    stateManager.reset();
    if (window) {
        glfwDestroyWindow(window);
        window = nullptr;
    }
    glfwTerminate();
    running = false;
    Logger::ShutDown();
}