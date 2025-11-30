#pragma once

#include "../IModuleLoader.hpp"
#include <functional>
#include <filesystem>

namespace Raindrop::Modules{
    class DynamicModuleLoader : public IModuleLoader{
        public:
            using CreateFnc = std::function<IModule*()>;
            using DestroyFnc = std::function<void(IModule*)>;

            DynamicModuleLoader(const std::filesystem::path& path);
            virtual ~DynamicModuleLoader() override;

            virtual std::shared_ptr<IModule> create() override;

        private:
            CreateFnc _create;
            DestroyFnc _destroy;
            void* _handle;

            std::filesystem::path readManifest(const std::filesystem::path& path);
            void* getSymbol(const char* name);
    };
}