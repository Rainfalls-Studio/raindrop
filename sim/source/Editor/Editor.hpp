#pragma once

#include "ComponentInfo.hpp"

namespace Editor {
    using Raindrop::Scene::Scene;
    using Raindrop::Scene::Entity;

    class Editor{
        public:
            Editor();

            void registerComponent(const ComponentInfo& component);
            const std::vector<ComponentInfo>& components() const;

            std::vector<std::shared_ptr<Scene>> scenes();
            void registerScene(std::shared_ptr<Scene> scene);

            Entity selected() const;
            void select(Entity entity);
        
        private:
            std::vector<ComponentInfo> _components;
            std::vector<std::weak_ptr<Scene>> _scenes;
            Entity _selected;


    };
}