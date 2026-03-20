#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include "render/IRenderAdapter.hpp"

class GameState {
    public:
        virtual ~GameState() = default;

        virtual void HandleInput() {}

        virtual void Enter() = 0;
        virtual void Exit() = 0;
        virtual void Update(float deltaTime) = 0;
        virtual void Render(IRenderAdapter& renderer) = 0;
};

#endif