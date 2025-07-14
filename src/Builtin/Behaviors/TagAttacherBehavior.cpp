#include "Raindrop/Builtin/Behaviors/TagAttacherBehavior.hpp"
#include "Raindrop/Builtin/Components/Tag.hpp"
#include "Raindrop/Scene/Entity.hpp"

namespace Raindrop::Builtin::Behaviors{
    void TagAttacherBehavior::onCreate(Scene::Entity& entity){
        auto& comp = entity.emplace<Components::Tag>();
        comp.tag = "Entity : " + std::to_string(entity.getHandle());
    }
}