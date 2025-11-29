#include "Scene/Behaviors/TransformAttacherBehavior.hpp"
#include "Scene/Components/Transform.hpp"
#include "Scene/Entity.hpp"

namespace Raindrop::Behaviors{
    void TransformAttacherBehavior::onCreate(Scene::Entity& entity){
        entity.emplace<Components::Transform>();
    }

    const char* TransformAttacherBehavior::name() const{
        return "Transform Attacher";
    }
}