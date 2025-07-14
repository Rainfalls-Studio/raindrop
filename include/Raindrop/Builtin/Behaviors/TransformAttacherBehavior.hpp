#pragma once

#include "Raindrop/Scene/IBehavior.hpp"

namespace Raindrop::Builtin::Behaviors{
    class TransformAttacherBehavior : public Scene::IBehavior{
        public:
            virtual ~TransformAttacherBehavior() override = default;

            virtual void onCreate(Scene::Entity& entity) override;
    };
}