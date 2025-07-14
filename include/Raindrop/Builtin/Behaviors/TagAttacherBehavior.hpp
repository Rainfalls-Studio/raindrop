#pragma once

#include "Raindrop/Scene/IBehavior.hpp"

namespace Raindrop::Builtin::Behaviors{
    class TagAttacherBehavior : public Scene::IBehavior{
        public:
            virtual ~TagAttacherBehavior() override = default;

            virtual void onCreate(Scene::Entity& entity) override;
    };
}