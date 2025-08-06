#pragma once

#include "Path.hpp"
#include "IProvider.hpp"

#include <list>
#include <vector>

namespace Raindrop::Filesystem{
    class MountTable{
        public:
            struct Entry{
                Path root;
                SharedProvider provider;
                int priority;
                bool readOnly;
            };

            using MountList = std::list<Entry>;

            MountTable();
            ~MountTable();

            void mount(const Path& root, SharedProvider provider, int priority);
            bool exists(const Path& path) const;
            SharedFile open(const Path& path, OpenFlags flags) const;

            const MountList& mounts(); 

        public:
            MountList _mounts;

            std::vector<const Entry*> findMatchingMounts(const Path& virtualPath) const;
    };
}