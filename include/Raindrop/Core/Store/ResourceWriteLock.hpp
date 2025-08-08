#pragma once

#include <memory>
#include "Types.hpp"
// #include "ResourceWriter.hpp"

namespace Raindrop::Store{
    template<typename T>
    class ResourceWriteLock {
        public:
            ResourceWriteLock() = delete;
            ResourceWriteLock(const ResourceWriteLock&) = delete;
            ResourceWriteLock& operator=(const ResourceWriteLock&) = delete;
            ResourceWriteLock(ResourceWriteLock&& o) noexcept
                : resource_(std::move(o.resource_)), idx_(o.idx_), released_(o.released_) {
                o.released_ = true;
            }
            ResourceWriteLock& operator=(ResourceWriteLock&&) = delete;

            // Access the buffer for writing
            T& get() {
                if (!resource_) throw std::runtime_error("invalid write lock");
                return resource_->buffers[idx_].data;
            }

            // On destruction: mark write finished and publish buffer as latest readable
            ~ResourceWriteLock() {
                if (released_) return;
                if (!resource_) return;
                std::lock_guard<std::mutex> lk(resource_->m);
                auto &buf = resource_->buffers[idx_];
                buf.writing = false;
                buf.version++;
                // mark this buffer as the latest readable
                resource_->latest_readable = idx_;
                resource_->buffers[idx_].ready = true;
                // mark other buffers as not-latest (but keep them readable if readers hold them)
                for (size_t i = 0; i < resource_->buffers.size(); ++i) {
                    if (i != idx_) resource_->buffers[i].ready = false;
                }
            }

        private:
            friend class ResourceWriter<T>;
            // private ctor only ResourceWriter can create locks
            ResourceWriteLock(std::shared_ptr<Resource<T>> resource, size_t idx)
                : resource_(std::move(resource)), idx_(idx), released_(false) {
                // buffer should already be marked as writing by Resource
            }

            std::shared_ptr<Resource<T>> resource_;
            size_t idx_;
            bool released_;
    };
}