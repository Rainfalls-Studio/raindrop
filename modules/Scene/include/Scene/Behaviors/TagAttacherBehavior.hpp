#pragma once

#include "Raindrop/Modules/Scene/IBehavior.hpp"

namespace Raindrop::Behaviors{
    class TagAttacherBehavior : public Scene::IBehavior{
        public:
            virtual ~TagAttacherBehavior() override = default;

            virtual void onCreate(Scene::Entity& entity) override;
            virtual const char* name() const override;
    };
}