#include "MenuState.hpp"
#include "logger/logger.hpp"

void MenuState::Enter() {
    Logger::Info("Entered MenuState");
}

void MenuState::Exit() {
    Logger::Info("Exited MenuState");
}

void MenuState::Update(float deltaTime) {
    // Logger::Info("MenuState::Update, deltaTime = " + std::to_string(deltaTime));
}

void MenuState::Render(IRenderAdapter& renderer) {

}