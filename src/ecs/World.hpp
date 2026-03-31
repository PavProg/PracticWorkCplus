#ifndef WORLD_HPP
#define WORLD_HPP

#include "Entity.hpp"
#include "Component.hpp"
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <vector>

class World {
    public:
        World();
        ~World();

        EntityId CreateEntity();
        void DestroyEntity(EntityId entity);

        template<typename T>
        bool HasComponent(EntityId entity) const;

        template<typename T>
        T& AddComponent(EntityId entity, T component);

        template<typename T>
        T& GetComponent(EntityId entity);

        template<typename T>
        void RemoveComponent(EntityId entity);

        template<typename T>
        const std::unordered_map<EntityId, T>& GetAllComponents() const;    // unodered_map = словарь = неупорядочный map
    private:
        EntityId m_nextEntityId;
        std::unordered_map<EntityId, bool> m_activeEntities;

        std::unordered_map<std::type_index, std::unique_ptr<void, void(*)(void*)>> m_componentPools;

        template<typename T>
        std::unordered_map<EntityId, T>& GetComponentPool();
};

#include "World.tpp"

#endif