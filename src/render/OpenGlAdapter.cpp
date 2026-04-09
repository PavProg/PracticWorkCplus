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
    : m_window(window),
      m_shaderProgram(0),
      m_vaoTriangle(0), m_vboTriangle(0),
      m_vaoSquare(0), m_vboSquare(0),
      m_triangleVertexCount(3), m_squareVertexCount(6),
      m_vertex_Path("/Users/pavel/Desktop/WorkingProject/CplusWorks/AID/Practice1/assets/shaders/triangle.vert"),
      m_fragmentPath("/Users/pavel/Desktop/WorkingProject/CplusWorks/AID/Practice1/assets/shaders/triangle.frag") {}

OpenGLAdapter::~OpenGLAdapter() {
    Shutdown();
}

bool OpenGLAdapter::Init() {
    Logger::Info("OpenGLAdapter::Init started");
    float triangleVertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };
    float squareVertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
    };
    Logger::Info("Init: vertices defined");
    
    //VAO и VBO для треугольника
    glGenVertexArrays(1, &m_vaoTriangle);
    glBindVertexArray(m_vaoTriangle);
    glGenBuffers(1, &m_vboTriangle);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboTriangle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // VAO и VBO для квадрата
    glGenVertexArrays(1, &m_vaoSquare);
    glBindVertexArray(m_vaoSquare);
    glGenBuffers(1, &m_vboSquare);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboSquare);
    glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Загрузка шейдеров
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

void OpenGLAdapter::DrawMesh(const glm::mat4& model, PrimitiveType type, const glm::vec4& color) {
    glUseProgram(m_shaderProgram);

    int modelLoc = glGetUniformLocation(m_shaderProgram, "uModel");
    int colorLoc = glGetUniformLocation(m_shaderProgram, "uColor");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
    glUniform4f(colorLoc, color.r, color.g, color.b, color.a);

    // VAO от типа примитива
    switch (type) {
        case PrimitiveType::Triangle:
            glBindVertexArray(m_vaoTriangle);
            glDrawArrays(GL_TRIANGLES, 0, m_triangleVertexCount);
            break;
        case PrimitiveType::Square:
            glBindVertexArray(m_vaoSquare);
            glDrawArrays(GL_TRIANGLES, 0, m_squareVertexCount);
            break;
        case PrimitiveType::Cube:
            Logger::Info("Cube type not yet initialization");
            break;
    }
    glBindVertexArray(0);
}

void OpenGLAdapter::SwapBuffers() {
    if (m_window) {
        glfwSwapBuffers(m_window);
    }
}

void OpenGLAdapter::Shutdown() {
    DeleteShaders();
    if (m_vaoTriangle) glDeleteVertexArrays(1, &m_vaoTriangle);
    if (m_vboTriangle) glDeleteBuffers(1, &m_vboTriangle);
    if (m_vaoSquare) glDeleteVertexArrays(1, &m_vaoSquare);
    if (m_vboSquare) glDeleteBuffers(1, &m_vboSquare);
    m_vaoTriangle = m_vboTriangle = m_vaoSquare = m_vboSquare = 0;
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
            uniform mat4 uModel;
            void main() {
                gl_Position = uModel * vec4(aPos, 1.0);
            }
        )";
        fragmentSrc = R"(
            #version 330 core
            uniform vec4 uColor;
            out vec4 FragColor;
            void main() {
                FragColor = uColor;
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