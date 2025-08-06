#include "Raindrop/Modules/Scene/Behaviors/HierarchyAttacherBehavior.hpp"
#include "Raindrop/Modules/Scene/Components/Hierarchy.hpp"
#include "Raindrop/Modules/Scene/Entity.hpp"

namespace Raindrop::Behaviors{
    void HierarchyAttacherBehavior::onCreate(Scene::Entity& entity){
        entity.emplace<Components::Hierarchy>();
    }
}