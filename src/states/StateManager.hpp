#ifndef STATEMANAGER_HPP
#define STATEMANAGER_HPP

#include <memory>
#include "GameState.hpp"
#include "render/IRenderAdapter.hpp"

class StateManager {
    public:
        StateManager();
        ~StateManager();

        void ChangeState(std::unique_ptr<GameState> newState);
        void Update(float deltaTime);
        void Render(IRenderAdapter& renderer);
    private:
        std::unique_ptr<GameState> currentState;
};

#endif