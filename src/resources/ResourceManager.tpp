#ifndef RESOURCE_MANAGER_TPP
#define RESOURCE_MANAGER_TPP

#include "logger/logger.hpp"

template<typename T>
std::shared_ptr<Resource<T>> ResourceManager::Load(const std::string& path) {
	std::type_index typeKey = std::type_index(typeid(T));
	auto& typeCache = m_caches[typeKey];

	auto it = typeCache.find(path);
	if (it != typeCache.end()) {
		if (std::shared_ptr<void> alive = it->second.lock()) {
			return std::static_pointer_cast<Resource<T>>(alive);
		}
		typeCache.erase(it);
	}

	std::shared_ptr<Resource<T>> resource = LoadImpl<T>(path);
	if (!resource) {
		Logger::Error("ResourceManager: failed to load " + path);
		return nullptr;
	}

	typeCache[path] = std::weak_ptr<void>(
		std::static_pointer_cast<void>(resource)
	);

	Logger::Info("ResourceManager: loaded " + path);
	return resource;
}

#endif