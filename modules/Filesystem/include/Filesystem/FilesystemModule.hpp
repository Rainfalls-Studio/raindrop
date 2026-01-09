#pragma once

#include "Raindrop/Modules/IModule.hpp"
#include "MountTable.hpp"

namespace Raindrop::Filesystem{
    class FilesystemModule : public IModule{
        public:

            enum class ErrorCodes{
                
            };

            FilesystemModule();
            virtual ~FilesystemModule() override = default;

            virtual Result initialize(InitHelper& helper) override;
            virtual void shutdown() override;

            MountTable& mountTable() noexcept;

            inline void mount(const Path& path, SharedProvider provider, int priority){
                mountTable().mount(path, provider, priority);
            }

            template<typename T>
            inline void mount(const Path& path, int priority){
                static_assert(std::is_base_of<IProvider, T>::value, "Provider must be derived from IProvider");
                mount(path, std::make_shared<T>(), priority);
            }

            template<typename T, typename... Args>
            inline void mount(const Path& path, int priority, Args&&... args){
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