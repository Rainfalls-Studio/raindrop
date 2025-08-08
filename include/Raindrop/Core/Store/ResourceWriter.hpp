#pragma once

#include <memory>
#include <unordered_set>
#include "Resource.hpp"

namespace Raindrop::Store{
    template<typename T>
    class ResourceWriter {
        public:
            ResourceWriter() = delete;
            ResourceWriter(const ResourceWriter&) = delete;
            ResourceWriter& operator=(const ResourceWriter&) = delete;

            // Try to create (take) the writer for a resource. If a writer already exists, returns nullopt.
            static std::optional<ResourceWriter> make_writer(std::shared_ptr<Resource<T>> res) {
                if (!res) return std::nullopt;
                std::lock_guard<std::mutex> lk(res->m);
                if (writers_taken.count(res.get())) {
                    return std::nullopt; // another writer exists
                }
                writers_taken.insert(res.get());
                return ResourceWriter(std::move(res));
            }

            // Non-blocking try to obtain an exclusive write lock. Returns nullopt if no writable buffer.
            std::optional<ResourceWriteLock<T>> try_lock() {
                return resource_->try_lock_write();
            }

            // Explicitly release writer (optional). Destructor will release automatically.
            void release_writer() {
                std::lock_guard<std::mutex> lk(resource_->m);
                auto it = writers_taken.find(resource_.get());
                if (it != writers_taken.end()) writers_taken.erase(it);
                resource_.reset();
            }

            ~ResourceWriter() {
                if (resource_) {
                    std::lock_guard<std::mutex> lk(resource_->m);
                    auto it = writers_taken.find(resource_.get());
                    if (it != writers_taken.end()) writers_taken.erase(it);
                    resource_.reset();
                }
            }

            // Move-only
            ResourceWriter(ResourceWriter&& o) noexcept : resource_(std::move(o.resource_)) {
                o.resource_.reset();
            }

            ResourceWriter& operator=(ResourceWriter&& o) = delete;

        private:
            explicit ResourceWriter(std::shared_ptr<Resource<T>> r) : resource_(std::move(r)) {}

            std::shared_ptr<Resource<T>> resource_;
            // Track resources that currently have a writer (global per-type). Using pointer identity is fine here.
            static inline std::unordered_set<Resource<T>*> writers_taken;
    };
}