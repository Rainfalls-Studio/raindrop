#pragma once

#include "Path.hpp"
#include "Types.hpp"
#include "IFile.hpp"

namespace Filesystem{
    class IFile;

    class IProvider{
        public:
            struct Properties{
                std::string name;
                bool readOnly;
            };

            virtual ~IProvider() = default;

            virtual SharedFile open(const Path& path, OpenFlags flags) = 0;
            virtual Properties getProperties() const = 0;
            virtual bool exists(const Path& path) = 0;
    };

    using SharedProvider = std::shared_ptr<IProvider>;
}