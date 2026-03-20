#include "StateManager.hpp"
#include "logger/logger.hpp"

StateManager::StateManager() : currentState(nullptr) {}

StateManager::~StateManager() {
    if (currentState) {
        currentState->Exit();
    }
}

void StateManager::ChangeState(std::unique_ptr<GameState> newState) {
    Logger::Info("ChangeState: entered");
    if (currentState) {
        Logger::Info("ChangeState: exiting old");
        currentState->Exit();
    }
    currentState = std::move(newState);
    if (currentState) {
        Logger::Info("ChangeState: about to call Enter");
        currentState->Enter();
        Logger::Info("ChangeState: after Enter");
    } else {
        Logger::Warning("StateManager: new state is null");
    }
    Logger::Info("ChangeState: finished");
}

void StateManager::Update(float deltaTime) {
    if (currentState) {
        currentState->Update(deltaTime);
    }
}

void StateManager::Render(IRenderAdapter& renderer) {
    if (currentState) {
        currentState->Render(renderer);
    }
}
