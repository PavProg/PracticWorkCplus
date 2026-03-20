#include "OpenGLAdapter.hpp"
#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "logger/logger.hpp"

// Ф-ция для чтения файла
static std::string ReadFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        return "";
    }
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

OpenGLAdapter::OpenGLAdapter(GLFWwindow* window) 
: m_window(window), m_vao(0), m_vbo(0), m_shaderProgram(0),
m_vertex_Path("/Users/pavel/Desktop/WorkingProject/CplusWorks/AID/Practice1/assets/shaders/triangle.vert"),
m_fragmentPath("/Users/pavel/Desktop/WorkingProject/CplusWorks/AID/Practice1/assets/shaders/triangle.frag") {}

OpenGLAdapter::~OpenGLAdapter() {
    Shutdown();
}

bool OpenGLAdapter::Init() {
    // Треугольник по xyz и rgb
    Logger::Info("OpenGLAdapter::Init started");
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
    };
    Logger::Info("Init: vertices defined");
    
    //VAO
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    Logger::Info("Init: VAO generated");

    // VBO
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    Logger::Info("Init: VBO generated");
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    Logger::Info("Init: Data loaded in buffer");

    // Позиция position = 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //цвет layout = 1
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    if (!LoadShaders()) {
        Logger::Error("LoadShader failed");
        return false;
    }
    Logger::Info("LoadShaders successfully");

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    Logger::Info("OpenGLAdapter::Init finished");
    return true;
}

void OpenGLAdapter::Clear(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLAdapter::DrawTriangle() {
    glUseProgram(m_shaderProgram);
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void OpenGLAdapter::SwapBuffers() {
    if (m_window) {
        glfwSwapBuffers(m_window);
    }
}

void OpenGLAdapter::Shutdown() {
    DeleteShaders();
    if (m_vao) glDeleteVertexArrays(1, &m_vao);
    if (m_vbo) glDeleteBuffers(1, &m_vbo);
    m_vao = 0;
    m_vbo = 0;
}

void OpenGLAdapter::ReloadShaders() {
    unsigned int newProgram = 0;

    std::string vertexSrc = ReadFile(m_vertex_Path);
    std::string fragmentSrc = ReadFile(m_fragmentPath);

    if (vertexSrc.empty() || fragmentSrc.empty()) {
        return;
    }

    unsigned int vertexShader = CompileShaders(GL_VERTEX_SHADER, vertexSrc);
    unsigned int fragmentShader = CompileShaders(GL_FRAGMENT_SHADER, fragmentSrc);

    if (!vertexShader || !fragmentShader) {
        return;
    }

    newProgram = glCreateProgram();
    glAttachShader(newProgram, vertexShader);
    glAttachShader(newProgram, fragmentShader);
    glLinkProgram(newProgram);

    int success;
    glGetProgramiv(newProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(newProgram, 512, nullptr, infoLog);
        glDeleteProgram(newProgram);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteProgram(m_shaderProgram);

    m_shaderProgram = newProgram;
}

bool OpenGLAdapter::LoadShaders() {
    std::string vertexSrc = ReadFile(m_vertex_Path);
    std::string fragmentSrc = ReadFile(m_fragmentPath);
    // Если файлов нет напрямую реализуем
    if (vertexSrc.empty() || fragmentSrc.empty()) {
        vertexSrc = R"(
            #version 330 core
            layout (location = 0) in vec3 aPos;
            layout (location = 1) in vec3 aColor;

            out vec3 Color;

            void main() {
            gl_Position = vec4(aPos, 1.0);
            Color = aColor;
            }
        )";
        fragmentSrc = R"(
            #version 330 core
            in vec3 Color;
            out vec4 FragColor;

            void main() {
                FragColor = vec4(Color, 1.0);
            }
        )";
    }

    unsigned int vertexShader = CompileShaders(GL_VERTEX_SHADER, vertexSrc);
    unsigned int fragmentShader = CompileShaders(GL_FRAGMENT_SHADER, fragmentSrc);
    if (!vertexShader || !fragmentShader) {
        return false;
    }

    m_shaderProgram = glCreateProgram();
    glAttachShader(m_shaderProgram, vertexShader);
    glAttachShader(m_shaderProgram, fragmentShader);
    glLinkProgram(m_shaderProgram);

    int success;
    glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(m_shaderProgram, 512, nullptr, infoLog);
        glDeleteProgram(m_shaderProgram);
        m_shaderProgram = 0;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return m_shaderProgram != 0;
}

unsigned int OpenGLAdapter::CompileShaders(unsigned int type, const std::string& source) {
    unsigned int shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

void OpenGLAdapter::DeleteShaders() {
    if (m_shaderProgram) {
        glDeleteProgram(m_shaderProgram);
        m_shaderProgram = 0;
    }
}