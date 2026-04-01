#include "PlayState.hpp"
#include "logger/logger.hpp"

void PlayState::Enter() {
    Logger::Info("Entered PlayState");
}

void PlayState::Exit() {
    Logger::Info("Exited PlayState");
}

void PlayState::Update(float deltaTime) {
    // Logger::Info("PlayState::Update, deltaTime = " + std::to_string(deltaTime));
}

void PlayState::Render(IRenderAdapter& renderer) {
    renderer.Clear(0.1f, 0.1f, 0.1f, 1.0f);
    renderer.DrawTriangle();
}