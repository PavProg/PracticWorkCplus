#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "application.hpp"
#include "logger/logger.hpp"

Application::Application() : window(nullptr), running(false), lastFrameTime(0.0) {}

Application::~Application() {
    Shutdown();
}

bool Application::Init(int width, int height, const char* title) {
    if (!glfwInit()) {
        Logger::Error("Failed to initialize GLFW");
        return false;
    }

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
    Logger::Init();
    Logger::Info("Application initialized");
    return true;
}

void Application::Run() {
    Logger::Info("Entering main loop");

    while (running && !glfwWindowShouldClose(window)) {
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastFrameTime;
        lastFrameTime = currentTime;

        Logger::Info("Delta time: " + std::to_string(deltaTime) + " seconds");

        glfwPollEvents();
    }
}

void Application::Shutdown() {
    Logger::Info("Shutting down application");
    if (window) {
        glfwDestroyWindow(window);
        window = nullptr;
    }
    glfwTerminate();
    running = false;
}