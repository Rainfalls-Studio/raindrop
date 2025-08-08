#pragma once

#include <memory>
#include "ResourceReadLock.hpp"
#include "Types.hpp"

namespace Raindrop::Store{
    template<typename T>
    class ResourceReader {
        public:
            explicit ResourceReader(std::shared_ptr<Resource<T>> r) : resource_(std::move(r)) {}
            ResourceReader() = delete;
            ResourceReader(const ResourceReader&) = delete;
            ResourceReader& operator=(const ResourceReader&) = delete;
            ResourceReader(ResourceReader&&) = default;

            // Non-blocking: returns a read lock to latest readable version or nullopt
            std::optional<ResourceReadLock<T>> try_lock() {
                if (!resource_) return std::nullopt;
                return resource_->try_lock_read();
            }

        private:
            std::shared_ptr<Resource<T>> resource_;
    };
}