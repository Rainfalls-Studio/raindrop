#pragma once

#include <memory>
#include <unordered_map>

#include "Result.hpp"
#include "Dependency.hpp"

namespace Raindrop::Modules{
    class InitHelper;
    class IModule{
        public:
            using Name = std::string;

            virtual ~IModule() = default;

            virtual Result initialize(InitHelper& helper) = 0;
            inline virtual void postInitialize() {}

            inline virtual void preShutdown() {}
            inline virtual void shutdown() {}

            virtual Name name() const noexcept = 0;
            inline virtual DependencyList dependencies() const noexcept {return {};}

            inline virtual Result dependencyReload(const Name&) {return Result::Level::SUCCESS;}
            inline virtual Result dependencyShutdown(const Name&) {return Result::Level::SUCCESS;}
            
            inline virtual bool critical() const noexcept {return false;}
    };

    using SharedModule = std::shared_ptr<IModule>;
    using WeakModule = std::shared_ptr<IModule>;
    using ModuleMap = std::unordered_map<IModule::Name, SharedModule>;
    using WeakModuleMap = std::unordered_map<IModule::Name, WeakModule>;
}