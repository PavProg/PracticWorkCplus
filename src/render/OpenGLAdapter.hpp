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
        void DrawTriangle() override;
        void SwapBuffers() override;
        void Shutdown() override;

        void ReloadShaders() override;
    private:
        GLFWwindow* m_window;

        unsigned int m_vao;
        unsigned int m_vbo;
        unsigned int m_shaderProgram;

        std::string m_vertex_Path;
        std::string m_fragmentPath;
        
        bool LoadShaders();
        unsigned int CompileShaders(unsigned int type, const std::string& source);
        void DeleteShaders();
};

#endif