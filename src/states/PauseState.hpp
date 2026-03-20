#ifndef PAUSESTATE_HPP
#define PAUSESTATE_HPP

#include "GameState.hpp"
#include "render/IRenderAdapter.hpp"

class PauseState : public GameState {
    public:
        void Enter() override;
        void Exit() override;
        void Update(float deltaTime) override;
        void Render(IRenderAdapter& renderer) override;
};

#endif