#pragma once

#include <memory>
#include <atomic>
#include <filesystem>
#include <spdlog/logger.h>

#include "IModule.hpp"
#include "Status.hpp"
#include "IModuleInstance.hpp"
#include "Instances/StaticModuleInstance.hpp"

namespace Raindrop{
    class Engine;
}

namespace Raindrop::Modules{
    class Manager{
        public:
            Manager(Engine& engine);
            ~Manager();

            void registerModule(std::unique_ptr<IModuleInstance>&& loader);

            template<typename T>
            void registerModule(const Name& name, const Version& version, DependencyList dependencies, bool critical = false){
                registerModule(
                    std::make_unique<StaticModuleInstance>(
                        []() -> IModule* { return new T();},
                        [](IModule* module) -> void {delete module;},
                        name,
                        version,
                        std::move(dependencies),
                        critical
                    )
                );
            }

            template<typename T, typename... Args>
            void registerModule(const Name& name, const Version& version, const DependencyList& dependencies, bool critical, Args&&... args){
                registerModule(
                    std::make_unique<StaticModuleInstance>(
                        [args...]() -> IModule* { return new T(std::forward<Args>(args)...);},
                        [](IModule* module) -> void {delete module;},
                        name,
                        version,
                        std::move(dependencies),
                        critical
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
            
            void initialize();
            void shutdown();

            void shutdownModule(Name module);

        private:
            struct Node{
                std::list<Name> dependents = {};
                std::atomic<Status> status = Status::UNREGISTRED;
                std::unique_ptr<IModuleInstance> instance = {};

                inline Name name() const noexcept{
                    return instance ? instance->name() : "";
                }
            };

            using Map = std::unordered_map<Name, Node>;

            Engine& _engine;
            Map _nodes;
            mutable std::mutex _mtx;
            std::shared_ptr<spdlog::logger> _logger;
            
            void shutdownModuleNode(Node& node);
            void tryInitializeModule(Node& node);
            Status catchResultError(const Name& name, const Result& result);
            void initializeModule(Node& node);
    };
}