#include "Raindrop/Builtin/Behaviors/TransformAttacherBehavior.hpp"
#include "Raindrop/Builtin/Components/Transform.hpp"
#include "Raindrop/Scene/Entity.hpp"

namespace Raindrop::Builtin::Behaviors{
    void TransformAttacherBehavior::onCreate(Scene::Entity& entity){
        entity.emplace<Components::Transform>();
    }
}