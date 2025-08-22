#pragma once

#include <memory>
#include "IModule.hpp"
#include "Status.hpp"

namespace Raindrop{
    class Engine;
}

namespace Raindrop::Modules{
    class Manager{
        public:
            Manager(Engine& engine);
            ~Manager();

            void registerModule(const SharedModule& module);

            template<typename T>
            void registerModule(){
                registerModule(std::make_shared<T>());
            }

            template<typename T, typename... Args>
            void registerModule(Args&&... args){
                registerModule(std::make_shared<T>(std::forward<Args>(args)...));
            }

            [[nodiscard]] SharedModule getModule(const IModule::Name& name) noexcept;

            template<typename... Modules>
            [[nodiscard]] auto getModules(const Modules&... modules){
                static_assert((std::is_same_v<Modules, const IModule::Name&> && ...), "The Modules names must be strings");
                return std::make_tuple((getModule(modules))...);
            }
            
            
            template<typename T>
            [[nodiscard]] std::shared_ptr<T> getModuleAs(const IModule::Name& name){
                return std::dynamic_pointer_cast<T>(getModule(name));
            }

            template<typename... Ts, typename... Modules>
            [[nodiscard]] auto getModulesAs(Modules&&... modules){
                static_assert(sizeof...(Ts) == sizeof...(Modules), "Types and names must match in count");
                static_assert((std::is_constructible<IModule::Name, Modules>::value && ...), "The Modules names must be strings");
                return std::make_tuple(getModuleAs<Ts>(IModule::Name(modules))...);
            }

            void shutdown();

        private:
            struct Node{
                SharedModule module = {};
                DependencyList dependencies = {};
                DependencyList dependents = {};
                Status status = Status::UNREGISTRED;
                uint32_t version = 0;

                inline IModule::Name name() const noexcept{
                    return module ? module->name() : "";
                }
            };

            using Map = std::unordered_map<IModule::Name, Node>;

            Engine& _engine;
            Map _nodes;
            uint32_t _version = 0;

            void initializeModule(Node& node);
            bool areModuleDependenciesMet(Node& node);
            Status catchResultError(const IModule::Name& name, const Result& result);
            void propagateInitialization(Node& source);
    };
}