#pragma once

#include "Types.hpp"
#include <memory>

namespace Raindrop::Store{
    template<typename T>
    class ResourceReadLock {
        public:
            ResourceReadLock() = delete;
            ResourceReadLock(const ResourceReadLock&) = delete;
            ResourceReadLock& operator=(const ResourceReadLock&) = delete;
            ResourceReadLock(ResourceReadLock&& o) noexcept
                : resource_(std::move(o.resource_)), idx_(o.idx_), released_(o.released_) {
                o.released_ = true;
            }
            ResourceReadLock& operator=(ResourceReadLock&&) = delete;

            // const access
            const T& get() const {
                if (!resource_) throw std::runtime_error("invalid read lock");
                return resource_->buffers[idx_].data;
            }

            // destructor: decrement reader count
            ~ResourceReadLock() {
                if (released_) return;
                if (!resource_) return;
                std::lock_guard<std::mutex> lk(resource_->m);
                auto &buf = resource_->buffers[idx_];
                if (buf.readers == 0) {
                    // shouldn't happen
                    return;
                }
                buf.readers--;
            }

        private:
            friend class ResourceReader<T>;
            ResourceReadLock(std::shared_ptr<Resource<T>> resource, size_t idx)
                : resource_(std::move(resource)), idx_(idx), released_(false) {}

            std::shared_ptr<Resource<T>> resource_;
            size_t idx_;
            bool released_;
    };
}