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

    CreateTestScene(*world);

    return true;
}

void Application::CreateTestScene(World& world) {
    // Сущность 1 - красный треугольник
    EntityId e1 = world.CreateEntity();
    world.AddComponent<Transform>(e1, {
        glm::vec3(-2.0f, 0.0f, 0.0f),
        glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 1.0f)
    });
    world.AddComponent<MeshRenderer>(e1, {
        PrimitiveType::Triangle,
        glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)
    });
    world.AddComponent<Tag>(e1, {"RedTriangle"});

    // Сущность2 - зеленый квадрат в центре
    EntityId e2 = world.CreateEntity();
    world.AddComponent<Transform>(e2, {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 1.0f)
    });
    world.AddComponent<MeshRenderer>(e2, {
        PrimitiveType::Square,
        glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)
    });
    world.AddComponent<Tag>(e2, {"GreenSquare"});

    //Сущность 3 - Синий треугольник справа
    EntityId e3 = world.CreateEntity();
    world.AddComponent<Transform>(e3, {
        glm::vec3(2.0f, 0.0f, 0.0f),
        glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
        glm::vec3(0.5f, 0.5f, 0.5f)
    });
    world.AddComponent<MeshRenderer>(e3, {
        PrimitiveType::Triangle,
        glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)
    });
    world.AddComponent<Tag>(e3, {"BlueTriangle"});

    //Сущность 4 - желтый квадрат сверху
    EntityId e4 = world.CreateEntity();
    world.AddComponent<Transform>(e4, {
        glm::vec3(0.0f, 1.5f, 0.0f),
        glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
        glm::vec3(0.8f, 0.8f, 1.0f)
    });
    world.AddComponent<MeshRenderer>(e4, {
        PrimitiveType::Square,
        glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)
    });
    world.AddComponent<Tag>(e4, {"YellowSquare"});
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

        renderer->Clear(0.0f, 0.0f, 0.0f, 1.0f);

        glm::mat4 view = glm::lookAt(
            glm::vec3(0.0f, 0.0f, 5.0f),    // Камера в мире
            glm::vec3(0.0f, 0.0f, 0.0f),    // Куда смотреть (центр)
            glm::vec3(0.0f, 1.0f, 0.0f)     // вектор вверх по y
        );

        glm::mat4 projection = glm::perspective(
            glm::radians(45.0f),    // Угол обзора в радианах
            aspect,
            0.1f,
            100.f   // объекты ближе 0.1 и дальше 100 отсекаются
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