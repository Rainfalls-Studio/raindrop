#pragma once

#include "Raindrop/Modules/Scene/IBehavior.hpp"

namespace Raindrop::Behaviors{
    class TransformAttacherBehavior : public Scene::IBehavior{
        public:
            virtual ~TransformAttacherBehavior() override = default;

            virtual void onCreate(Scene::Entity& entity) override;
            virtual const char* name() const override;
    };
}