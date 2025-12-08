#pragma once

#include "../IModuleInstance.hpp"
#include <functional>

namespace Raindrop::Modules{
    class StaticModuleInstance : public IModuleInstance{
        public:
            using CreateFnc = std::function<IModule*()>;
            using DestroyFnc = std::function<void(IModule*)>;

            StaticModuleInstance(
                CreateFnc create,
                DestroyFnc destroy,
                Name name,
                Version version,
                DependencyList dependencies,
                bool critical = false
            );
            virtual ~StaticModuleInstance() override = default;

            virtual const DependencyList& dependencies() const override;
            virtual const Name& name() const override;
            virtual const Version& version() const override;
            virtual bool critical() const override;

            virtual Result initialize(InitHelper& helper) override;
            virtual void shutdown() override;

            virtual std::shared_ptr<IModule> module() const override;

        private:
            CreateFnc _create;
            DestroyFnc _destroy;
            Name _name;
            Version _version;
            DependencyList _dependencies;
            bool _critical;
            
            std::shared_ptr<IModule> _module;
    };
}