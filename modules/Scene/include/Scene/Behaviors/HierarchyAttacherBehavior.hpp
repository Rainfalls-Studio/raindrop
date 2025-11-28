#pragma once

#include "Raindrop/Modules/Scene/IBehavior.hpp"

namespace Raindrop::Behaviors{
    class HierarchyAttacherBehavior : public Scene::IBehavior{
        public:
            virtual ~HierarchyAttacherBehavior() override = default;

            virtual void onCreate(Scene::Entity& entity) override;
            virtual const char* name() const override;
    };
}