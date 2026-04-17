#ifndef HIERARCHY_HPP
#define HIERARCHY_HPP

#include "ecs/Entity.hpp"
#include <vector>

struct Hierarchy {
    EntityId parent = INVALID_ID;
    std::vector<EntityId> children;
};

#endif