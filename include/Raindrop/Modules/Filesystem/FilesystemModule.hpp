#pragma once

#include "Raindrop/Core/Modules/IModule.hpp"
#include "MountTable.hpp"

namespace Raindrop::Filesystem{
    class FilesystemModule : public Modules::IModule{
        public:
            FilesystemModule();
            virtual ~FilesystemModule() override = default;

            virtual Modules::Result initialize(Modules::InitHelper& helper) override;
            virtual void shutdown() override;

            virtual Name name() const noexcept override;

            MountTable& mountTable() noexcept;

            void mount(const Path& path, SharedProvider provider, int priority){
                mountTable().mount(path, provider, priority);
            }

            template<typename T>
            void mount(const Path& path, int priority){
                static_assert(std::is_base_of<IProvider, T>::value, "Provider must be derived from IProvider");
                mount(path, std::make_shared<T>(), priority);
            }

            template<typename T, typename... Args>
            void mount(const Path& path, int priority, Args&&... args){
                static_assert(std::is_base_of<IProvider, T>::value, "Provider must be derived from IProvider");
                mount(path, std::make_shared<T>(std::forward<Args>(args)...), priority);
            }

            bool exists(const Path& path) const;
            SharedFile open(const Path& path, OpenFlags flags) const;

            static Path getExecutableDirectory();

        private:
            MountTable _mounts;
    };
}