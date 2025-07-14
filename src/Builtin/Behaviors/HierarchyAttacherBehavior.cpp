#include "Raindrop/Builtin/Behaviors/HierarchyAttacherBehavior.hpp"
#include "Raindrop/Builtin/Components/Hierarchy.hpp"
#include "Raindrop/Scene/Entity.hpp"

namespace Raindrop::Builtin::Behaviors{
    void HierarchyAttacherBehavior::onCreate(Scene::Entity& entity){
        entity.emplace<Components::Hierarchy>();
    }
}