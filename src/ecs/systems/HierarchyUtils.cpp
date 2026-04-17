#include "HierarchyUtils.hpp"
#include "ecs/components/Hierarchy.hpp"

#include <algorithm>

namespace HierarchyUtils {
    static Hierarchy& GetOrCreateHierarchy(World& world, EntityId entity) {
        if (!world.HasComponent<Hierarchy>(entity)) {
            world.AddComponent<Hierarchy>(entity, Hierarchy{});
        }
        return world.GetComponent<Hierarchy>(entity);
    }

    void SetParent(World& world, EntityId child, EntityId parent) {
        if (child == parent) {
            return;
        }

        Hierarchy& childHierarchy = GetOrCreateHierarchy(world, child);

        if (childHierarchy.parent != INVALID_ID) {
            EntityId oldParent = childHierarchy.parent;
            if (world.HasComponent<Hierarchy>(oldParent)) {
                auto& oldParentHierarchy = world.GetComponent<Hierarchy>(oldParent);
                auto& kids = oldParentHierarchy.children;
                kids.erase(std::remove(kids.begin(), kids.end(), child), kids.end());
            }
        }
        childHierarchy.parent = parent;

        if (parent == INVALID_ID) {
            return;
        }

        Hierarchy& parentHierarchy = GetOrCreateHierarchy(world, parent);
        parentHierarchy.children.push_back(child);
    }
}