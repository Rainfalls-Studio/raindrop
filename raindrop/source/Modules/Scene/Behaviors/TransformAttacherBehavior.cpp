#include "Raindrop/Modules/Scene/Behaviors/TransformAttacherBehavior.hpp"
#include "Raindrop/Modules/Scene/Components/Transform.hpp"
#include "Raindrop/Modules/Scene/Entity.hpp"

namespace Raindrop::Behaviors{
    void TransformAttacherBehavior::onCreate(Scene::Entity& entity){
        entity.emplace<Components::Transform>();
    }

    const char* TransformAttacherBehavior::name() const{
        return "Transform Attacher";
    }
}