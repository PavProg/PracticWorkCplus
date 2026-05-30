#include "ResourceManager.hpp"
#include "logger/logger.hpp"

ResourceManager::ResourceManager(IRenderAdapter& renderer) 
	: m_renderer(renderer) {}

ResourceManager::~ResourceManager() {
	Clear();
}

void ResourceManager::Clear() {
	m_caches.clear();
	Logger::Info("ResourceManager: cleared");
}