#pragma once

#include "../IModuleLoader.hpp"
#include <functional>

namespace Raindrop::Modules{
    class StaticModuleLoader : public IModuleLoader{
        public:
            using CreateFnc = std::function<IModule*()>;
            using DestroyFnc = std::function<void(IModule*)>;

            StaticModuleLoader(CreateFnc create, DestroyFnc destroy);
            virtual ~StaticModuleLoader() override = default;

            virtual std::shared_ptr<IModule> create() override;

        private:
            CreateFnc _create;
            DestroyFnc _destroy;
    };
}