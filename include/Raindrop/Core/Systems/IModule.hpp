#pragma once

#include <vector>
#include <expected>

#include "../Error.hpp"
#include "Dependency.hpp"

namespace Raindrop::Systems{

    class IModule{
        public:
            virtual ~IModule() = default;

            virtual std::expected<void, Error> initialize() = 0;
            inline virtual void postInitialize() {}

            inline virtual void preShutdown() {}
            virtual void shutdown() = 0;

            virtual std::string name() const noexcept = 0;
            virtual std::string systemName() const noexcept = 0;
            inline virtual std::vector<Dependency> dependencies() const noexcept {return {};}
    };
}