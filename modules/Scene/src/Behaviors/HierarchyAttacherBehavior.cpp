#include "Scene/Behaviors/HierarchyAttacherBehavior.hpp"
#include "Scene/Components/Hierarchy.hpp"
#include "Scene/Entity.hpp"

namespace Raindrop::Behaviors{
    void HierarchyAttacherBehavior::onCreate(Scene::Entity& entity){
        entity.emplace<Components::Hierarchy>();
    }

    const char* HierarchyAttacherBehavior::name() const{
        return "Hierarchy Attacher";
    }
}