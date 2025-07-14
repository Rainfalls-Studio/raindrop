#pragma once

#include "Raindrop/Scene/IBehavior.hpp"

namespace Raindrop::Builtin::Behaviors{
    class HierarchyAttacherBehavior : public Scene::IBehavior{
        public:
            virtual ~HierarchyAttacherBehavior() override = default;

            virtual void onCreate(Scene::Entity& entity) override;
    };
}