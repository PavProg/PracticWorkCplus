#ifndef MENUSTATE_HPP
#define MENUSTATE_HPP

#include "GameState.hpp"
#include "render/IRenderAdapter.hpp"

class MenuState : public GameState {
    public:
        void Enter() override;
        void Exit() override;
        void Update(float deltaTime) override;
        void Render(IRenderAdapter& renderer) override;
};

#endif