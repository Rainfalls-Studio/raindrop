#pragma once

#include <memory>
#include "IModule.hpp"

namespace Raindrop::Modules{
    class IModuleInstance{
        public:
            virtual ~IModuleInstance() = default;
            
            virtual const DependencyList& dependencies() const = 0;
            virtual const Name& name() const = 0;
            virtual const Version& version() const = 0;
            virtual bool critical() const = 0;

            virtual Result initialize(InitHelper& helper) = 0;
            virtual void shutdown() = 0;

            virtual std::shared_ptr<IModule> module() const = 0;
    };
}