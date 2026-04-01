#ifndef WORLD_TPP
#define WORLD_TPP

template<typename T>
std::unordered_map<EntityId, T>& World::GetComponentPool() {
    std::type_index type = std::type_index(typeid(T));
    auto it = m_componentPools.find(type);
    if (it == m_componentPools.end()) {
        auto pool = std::make_unique<std::unordered_map<EntityId, T>>();
        auto result = m_componentPools.emplace(
            type,
            std::unique_ptr<void, void(*)(void*)>(
                pool.release(),
                [](void* ptr) { delete static_cast<std::unordered_map<EntityId, T>*>(ptr); }
            )
        );
        it = result.first;
    }
    return *static_cast<std::unordered_map<EntityId, T>*>(it->second.get());
}

template<typename T>
bool World::HasComponent(EntityId entity) const {
    std::type_index type = std::type_index(typeid(T));
    auto it = m_componentPools.find(type);
    if (it == m_componentPools.end()) return false;
    const auto& map = *static_cast<const std::unordered_map<EntityId, T>*>(it->second.get());
    return map.find(entity) != map.end();
}

template<typename T>
T& World::AddComponent(EntityId entity, T component) {
    auto& pool = GetComponentPool<T>();
    pool[entity] = std::move(component);
    return pool[entity];
}

template<typename T>
T& World::GetComponent(EntityId entity) {
    auto& pool = GetComponentPool<T>();
    return pool.at(entity);
}

template<typename T>
void World::RemoveComponent(EntityId entity) {
    auto& pool = GetComponentPool<T>();
    pool.erase(entity);
}

template<typename T>
const std::unordered_map<EntityId, T>& World::GetAllComponents() const {
    std::type_index type = std::type_index(typeid(T));
    static std::unordered_map<EntityId, T> empty;
    auto it = m_componentPools.find(type);
    if (it == m_componentPools.end()) return empty;
    return *static_cast<const std::unordered_map<EntityId, T>*>(it->second.get());
}

#endif