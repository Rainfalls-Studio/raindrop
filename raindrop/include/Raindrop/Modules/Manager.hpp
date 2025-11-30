#pragma once

#include <memory>
#include <atomic>
#include <filesystem>

#include "IModule.hpp"
#include "Status.hpp"
#include "IModuleLoader.hpp"
#include "Loaders/StaticModuleLoader.hpp"

namespace Raindrop{
    class Engine;
}

namespace Raindrop::Modules{
    class Manager{
        public:
            Manager(Engine& engine);
            ~Manager();

            void registerModule(std::unique_ptr<IModuleLoader>&& loader);

            template<typename T>
            void registerModule(){
                registerModule(
                    std::make_unique<StaticModuleLoader>(
                        []() -> IModule* { return new T();},
                        [](IModule* module) -> void {delete module;}
                    )
                );
            }

            template<typename T, typename... Args>
            void registerModule(Args&&... args){
                registerModule(
                    std::make_unique<StaticModuleLoader>(
                        [args...]() -> IModule* { return new T(std::forward<Args>(args)...);},
                        [](IModule* module) -> void {delete module;}
                    )
                );
            }

            void loadModule(const std::filesystem::path& path);
            void loadModules(const std::filesystem::path& directoryPath);

            [[nodiscard]] SharedModule getModule(const Name& name) noexcept;

            template<typename... Modules>
            [[nodiscard]] auto getModules(const Modules&... modules){
                static_assert((std::is_same_v<Modules, const Name&> && ...), "The Modules names must be strings");
                return std::make_tuple((getModule(modules))...);
            }
            
            
            template<typename T>
            [[nodiscard]] std::shared_ptr<T> getModuleAs(const Name& name){
                return std::dynamic_pointer_cast<T>(getModule(name));
            }

            template<typename... Ts, typename... Modules>
            [[nodiscard]] auto getModulesAs(Modules&&... modules){
                static_assert(sizeof...(Ts) == sizeof...(Modules), "Types and names must match in count");
                static_assert((std::is_constructible<Name, Modules>::value && ...), "The Modules names must be strings");
                return std::make_tuple(getModuleAs<Ts>(Name(modules))...);
            }

            void shutdown();

        private:
            struct Node{
                SharedModule module = {};
                DependencyList dependencies = {};
                std::list<Name> dependents = {};
                std::atomic<Status> status = Status::UNREGISTRED;
                std::unique_ptr<IModuleLoader> loader = {};

                inline Name name() const noexcept{
                    return module ? module->name() : "";
                }
            };

            using Map = std::unordered_map<Name, Node>;

            Engine& _engine;
            Map _nodes;
            mutable std::mutex _mtx;

            void initializeModule(Node& node);
            bool areModuleDependenciesMet(Node& node);
            Status catchResultError(const Name& name, const Result& result);
            void propagateInitialization(Node& source);
    };
}