#ifndef HIERARCHY_UTILS_HPP
#define HIERARCHY_UTILS_HPP

#include "ecs/World.hpp"
#include "ecs/Entity.hpp"

// Пространство имен чтобы не загружать World
namespace HierarchyUtils {
    void SetParent(World& world, EntityId child, EntityId parent);
}

#endif