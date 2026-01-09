#pragma once

#include "Types.hpp"
#include <memory>

namespace Filesystem{
    class IFile{
        public:
            virtual ~IFile() = default;

            virtual void read(char* buffer, Size size) = 0;
            virtual void seek(Offset offset, SeekOrigin origin) = 0;
            virtual Size tell() = 0;
            virtual Size size() = 0;

            virtual void close() = 0;
    };

    using SharedFile = std::shared_ptr<IFile>;
}