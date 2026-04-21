#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "application.hpp"
#include "logger/logger.hpp"
#include "states/StateManager.hpp"
#include "states/MenuState.hpp"
#include "states/PlayState.hpp"
#include "states/PauseState.hpp"
#include "render/OpenGLAdapter.hpp"
#include "ecs/World.hpp"
#include "ecs/components/Transform.hpp"
#include "ecs/components/Tag.hpp"
#include "ecs/components/MeshRenderer.hpp"
#include "ecs/systems/RenderSystem.hpp"
#include "ecs/systems/AnimationSystem.hpp"
#include "ecs/components/Hierarchy.hpp"
#include "ecs/systems/HierarchyUtils.hpp"
#include "scene/SceneLoader.hpp"

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
    world = std::make_unique<World>();
    Logger::Info("Build Marker v2: creating systems");
    renderSystem = std::make_unique<RenderSystem>(*world, *renderer);
    Logger::Info("RenderSystem created");
    animationSystem = std::make_unique<AnimationSystem>(*world);
    Logger::Info("AnimationSystem created");

    if (!SceneLoader::Load("assets/scenes/test_scene.json", *world, m_sceneSettings)) {
        Logger::Error("Failed to load scene; using empty world");
    }

    return true;
}

void Application::Run() {
    Logger::Info("Entering main loop");
    lastFrameTime = glfwGetTime();
    while (running && !glfwWindowShouldClose(window)) {
        double currentTime = glfwGetTime();
        float deltaTime = static_cast<float>(currentTime - lastFrameTime);
        lastFrameTime = currentTime;

        glfwPollEvents();

        if (stateManager) {
            stateManager->Update(deltaTime);
        }

        // Настрока камеры для этого кадра
        int fbWidth, fbHeight;
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
        glViewport(0, 0, fbWidth, fbHeight);

        float aspect = (fbHeight > 0)
            ? static_cast<float>(fbWidth) / static_cast<float>(fbHeight)
            : 1.0f;

        renderer->Clear(
            m_sceneSettings.background.r,
            m_sceneSettings.background.g,
            m_sceneSettings.background.b,
            m_sceneSettings.background.a
        );

        glm::mat4 view = glm::lookAt(
            m_sceneSettings.camera.position,
            m_sceneSettings.camera.target,
            m_sceneSettings.camera.up
        );

        glm::mat4 projection = glm::perspective(
            glm::radians(m_sceneSettings.camera.fov),
            aspect,
            m_sceneSettings.camera.nearPlan,
            m_sceneSettings.camera.farPlan
        );

        renderer->SetCamera(view, projection);

        if (animationSystem) {
            animationSystem->Update(deltaTime);
        } else {
            Logger::Info("animationSystem is NULL!");
        }

        // Отрисовка через Ecs
        if (renderSystem) {
            renderSystem->Update();
        }

        if (stateManager) {
            stateManager->Render(*renderer);
        }

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