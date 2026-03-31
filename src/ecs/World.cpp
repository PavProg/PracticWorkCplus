#include "World.hpp"
#include <algorithm>

World::World() : m_nextEntityId(1) {}

World::~World() {}

EntityId World::CreateEntity() {
    EntityId id = m_nextEntityId++;
    m_activeEntities[id] = true;
    return id;
}

void World::DestroyEntity(EntityId entity) {
    auto it = m_activeEntities.find(entity);
    if (it == m_activeEntities.end()) return;
    m_activeEntities.erase(it);

    for (auto& pool : m_componentPools) {
        using MapType = std::unordered_map<EntityId, void*>;
        MapType* map = static_cast<MapType*>(pool.second.get());
        map->erase(entity);
    }
}

template<typename T>
std::unordered_map<EntityId, T>& World::GetComponentPool() {
    std::type_index type = std::type_index(typeid(T));
    auto it = m_activeEntities.find(type);
    if (it == m_activeEntities.end()) {
        auto pool = std::make_unique<std::unordered_map<EntityId, T>>();
        m_componentPools[type] = std::unique_ptr<void, void(*)(void*)>(
            pool.release(),
            [](void* ptr) { delete static_cast<std::unordered_map<EntityId, T>*>(ptr); }
        );
    }
    auto& poolPtr = m_componentPools[type];
    return *static_cast<std::unordered_map<EntityId, T>*>(poolPtr.get());
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