#ifndef PLAYSTATE_HPP
#define PLAYSTATE_HPP

#include "GameState.hpp"
#include "render/IRenderAdapter.hpp"

class PlayState : public GameState {
    public:
        void Enter() override;
        void Exit() override;
        void Update(float deltaTime) override;
        void Render(IRenderAdapter& renderer) override;
};

#endif