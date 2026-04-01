#include "World.hpp"

World::World() : m_nextEntityId(1) {}
World::~World() {}

EntityId World::CreateEntity() {
    EntityId id = m_nextEntityId++;
    m_activeEntities[id] = true;
    return id;
}

void World::DestroyEntity(EntityId entity) {
    m_activeEntities.erase(entity);
}