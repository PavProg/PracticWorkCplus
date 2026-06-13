#ifndef IRENDERADAPTER_HPP
#define IRENDERADAPTER_HPP

#include "glm/glm.hpp"
#include "ecs/components/MeshRenderer.hpp"
#include "resources/Mesh.hpp"
#include "resources/Texture.hpp"
#include "resources/ShaderProgram.hpp"

class IRenderAdapter {
public:
    virtual ~IRenderAdapter() = default;

    virtual bool Init() = 0;
    virtual void Clear(float r, float g, float b, float a) = 0;
    virtual void SwapBuffers() = 0;
    virtual void Shutdown() = 0;

    virtual void DrawMesh(const glm::mat4& model, PrimitiveType type, const glm::vec4& color) = 0;

    virtual void SetCamera(const glm::mat4& view, const glm::mat4& projection) = 0;

    virtual void ReloadShaders() = 0;

    virtual GPUMesh UploadMesh(const MeshData& data) = 0;
    virtual void ReleaseMesh(GPUMesh& mesh) = 0;

    virtual GPUTexture UploadTexture(const TextureData& data) = 0;
    virtual void ReleaseTexture(GPUTexture& tex) = 0;

    virtual GPUShader CompileShader(const std::string& vertexSrc, const std::string& fragmentSrc) = 0;
    virtual void ReleaseShader(GPUShader& shader) = 0;

    virtual void DrawLoadedMesh(const GPUMesh& mesh, const GPUTexture& texture, const GPUShader& shader, 
        const glm::mat4& model, const glm::vec4& tint) = 0;
};

#endif