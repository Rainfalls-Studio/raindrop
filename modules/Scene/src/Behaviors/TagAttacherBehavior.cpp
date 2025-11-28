#include "Raindrop/Modules/Scene/Behaviors/TagAttacherBehavior.hpp"
#include "Raindrop/Modules/Scene/Components/Tag.hpp"
#include "Raindrop/Modules/Scene/Entity.hpp"

namespace Raindrop::Behaviors{
    void TagAttacherBehavior::onCreate(Scene::Entity& entity){
        auto& comp = entity.emplace<Components::Tag>();
        comp.tag = "Entity : " + std::to_string(entity.getHandle());
    }

    const char* TagAttacherBehavior::name() const{
        return "Tag Attacher";
    }
}