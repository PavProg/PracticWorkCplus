#include "PauseState.hpp"
#include "logger/logger.hpp"

void PauseState::Enter() {
    Logger::Info("Entered PauseState");
}

void PauseState::Exit() {
    Logger::Info("Exited PauseState");
}

void PauseState::Update(float deltaTime) {
    // Logger::Info("PauseState::Update, deltaTime = " + std::to_string(deltaTime));
}

void PauseState::Render(IRenderAdapter& renderer) {

}