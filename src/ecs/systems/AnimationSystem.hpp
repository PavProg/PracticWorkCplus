#ifndef ANIMATIONSYSTEM_HPP
#define ANIMATIONSYSTEM_HPP

#include "ecs/World.hpp"

class AnimationSystem {
    public:
        explicit AnimationSystem(World& world);
        void Update(float deltaTime);
    private:
        World& m_world;
        float m_totalTime;
};

#endif