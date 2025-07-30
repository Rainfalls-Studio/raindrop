#pragma once

#include <unordered_map>
#include "ISystem.hpp"

namespace Raindrop::Systems{
    class SystemGraph{
        public:
            struct RichNode{
                std::shared_ptr<ISystem> system;
                std::vector<std::string> dependencies;
                std::vector<std::string> dependents;

                inline decltype(auto) name() const{
                    return system ? system->name() : "None";
                }
            };

            using Map = std::unordered_map<std::string, RichNode>;

            SystemGraph();

            void push(const std::shared_ptr<ISystem>& system);
            void pop(const std::shared_ptr<ISystem>& system);
            
            // Pop a system and all the other systems directly or indirectly dependent of it.
            void popAndDependents(const std::shared_ptr<ISystem>& system);

            // clear all systems
            void clear();

            std::expected<void, Error> initialize(Engine& engine);
            void shutdown();

            const Map& getNodes();

        private:
            Map _nodes;
    };
}