#pragma once

#include "../IProvider.hpp"

namespace Filesystem{
    class FolderProvider : public IProvider{
        public:
            FolderProvider(const Path& root);
            virtual ~FolderProvider() = default;

            virtual SharedFile open(const Path& path, OpenFlags flags) override;
            virtual Properties getProperties() const override;
            virtual bool exists(const Path& path) override;

        private:
            Path _root;
    };
}