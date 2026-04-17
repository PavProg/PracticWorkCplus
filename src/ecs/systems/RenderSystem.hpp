#ifndef RENDERSYSTEM_HPP
#define RENDERSYSTEM_HPP

#include "ecs/World.hpp"
#include "render/IRenderAdapter.hpp"

class RenderSystem {
    public:
        RenderSystem(World& World, IRenderAdapter& IRenderAdapter);
        void Update();
    private:
        World& m_world;
        IRenderAdapter& m_renderer;

        glm::mat4 ComputeWorldMatrix(EntityId entity, int depth = 0);
};

#endif