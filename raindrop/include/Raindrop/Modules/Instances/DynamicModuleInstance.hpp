#pragma once

#include "../IModuleInstance.hpp"
#include <functional>
#include <filesystem>

namespace Raindrop::Modules{
    class DynamicModuleInstance : public IModuleInstance{
        public:
            using CreateFnc = std::function<IModule*()>;
            using DestroyFnc = std::function<void(IModule*)>;

            DynamicModuleInstance(const std::filesystem::path& path);
            virtual ~DynamicModuleInstance() override;

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
            DependencyList _dependencies;
            Name _name;
            std::string _description;
            Version _version;
            bool _critical;

            std::filesystem::path _libraryPath;
            std::filesystem::path _path;
            
            void* _handle;
            std::shared_ptr<IModule> _module;

            void readManifest(const std::filesystem::path& path);
            Result readLibrary();
            void* getSymbol(const char* name);
    };
}