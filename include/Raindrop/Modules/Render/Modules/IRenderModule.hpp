#pragma once

#include "Raindrop/Core/Modules/IModule.hpp"

namespace Raindrop::Render{
    class IRenderModule : public Modules::IModule{
        public:
            IRenderModule();
            virtual ~IRenderModule() override = default;

            virtual std::expected<void, Error> moduleInitialize() = 0;
            inline virtual void modulePostInitialize() {}

            inline virtual void modulePreShutdown() {}
            virtual void moduleShutdown() = 0;

            virtual std::string systemName() const noexcept final override;
    };
}