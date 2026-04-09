#ifndef OPENGLADAPTER_HPP
#define OPENGLADAPTER_HPP

#include "IRenderAdapter.hpp"
#include <string>
#include <chrono>

struct GLFWwindow;

class OpenGLAdapter : public IRenderAdapter {
    public:
        explicit OpenGLAdapter(GLFWwindow* window);
        ~OpenGLAdapter();

        bool Init() override;
        void Clear(float r, float g, float b, float a) override;
        void SwapBuffers() override;
        void Shutdown() override;

        void DrawMesh(const glm::mat4& model, PrimitiveType type, const glm::vec4& color) override;

        void ReloadShaders() override;
    private:
        GLFWwindow* m_window;

        unsigned int m_shaderProgram;

        unsigned int m_vaoTriangle;
        unsigned int m_vaoSquare;
        unsigned int m_vboTriangle;
        unsigned int m_vboSquare;

        int m_triangleVertexCount;
        int m_squareVertexCount;

        std::string m_vertex_Path;
        std::string m_fragmentPath;
        
        bool LoadShaders();
        unsigned int CompileShaders(unsigned int type, const std::string& source);
        void DeleteShaders();
};

#endif