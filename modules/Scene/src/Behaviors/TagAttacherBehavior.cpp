#include "Scene/Behaviors/TagAttacherBehavior.hpp"
#include "Scene/Components/Tag.hpp"
#include "Scene/Entity.hpp"

namespace Raindrop::Behaviors{
    void TagAttacherBehavior::onCreate(Scene::Entity& entity){
        auto& comp = entity.emplace<Components::Tag>();
        comp.tag = "Entity : " + std::to_string(entity.getHandle());
    }

    const char* TagAttacherBehavior::name() const{
        return "Tag Attacher";
    }
}