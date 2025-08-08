#pragma once

#include <memory>
#include "ResourceBuilder.hpp"
#include "ResourceWriteLock.hpp"
#include "ResourceReadLock.hpp"
#include "ResourceWriter.hpp"
#include "ResourceReader.hpp"


namespace Raindrop::Store{
    // The core Resource class, holds multiple buffers and synchronization state
    template<typename T>
    class Resource : public std::enable_shared_from_this<Resource<T>> {
        public:
            struct Buffer {
                T data;
                size_t readers = 0;      // number of active readers
                bool writing = false;    // currently being written
                bool ready = false;      // is readable (latest)
                uint64_t version = 0;    // monotonic per-buffer version
                Buffer() = default;
            };

            // Create a Resource with builder and 'count' buffers
            static std::shared_ptr<Resource<T>> create(const ResourceBuilder<T>& builder, size_t count) {
                if (count < 2) throw std::invalid_argument("count must be >= 2 for double-buffering");
                auto res = std::shared_ptr<Resource<T>>(new Resource<T>(builder, count));
                return res;
            }

            // Attempt to acquire a write lock (non-blocking). If none available, returns nullopt.
            std::optional<ResourceWriteLock<T>> try_lock_write() {
                std::lock_guard<std::mutex> lk(m);
                // find a buffer that is not being written and has no readers
                for (size_t i = 0; i < buffers.size(); ++i) {
                    auto &b = buffers[i];
                    if (!b.writing && b.readers == 0) {
                        // claim this buffer for writing
                        b.writing = true;
                        // When writing starts, mark not ready yet until writer finishes
                        b.ready = false;
                        return ResourceWriteLock<T>(this->shared_from_this(), i);
                    }
                }
                return std::nullopt;
            }

            // Attempt to acquire a read lock to the latest readable version (non-blocking)
            std::optional<ResourceReadLock<T>> try_lock_read() {
                std::lock_guard<std::mutex> lk(m);
                if (latest_readable >= buffers.size()) return std::nullopt;
                auto &b = buffers[latest_readable];
                if (!b.ready) return std::nullopt;
                // increment reader count and return lock
                b.readers++;
                return ResourceReadLock<T>(this->shared_from_this(), latest_readable);
            }

            // Internal helpers (used by writer/reader classes)
            size_t buffer_count() const { return buffers.size(); }

        private:
            friend class ResourceWriter<T>;
            friend class ResourceReader<T>;

            explicit Resource(const ResourceBuilder<T>& builder, size_t count)
                : buffers(count), latest_readable(count) // latest_readable invalid initially
            {
                for (size_t i = 0; i < count; ++i) {
                    buffers[i].data = builder.make();
                    buffers[i].readers = 0;
                    buffers[i].writing = false;
                    buffers[i].ready = false;
                    buffers[i].version = 0;
                }
                // Mark buffer 0 as initial readable immediately (conservative choice).
                // If you prefer no initial readable until writer publishes, leave them all not ready.
                latest_readable = 0;
                buffers[0].ready = true;
            }

            mutable std::mutex m;
            std::vector<Buffer> buffers;
            size_t latest_readable; // index of latest readable buffer (buffers[latest_readable].ready must be true)
        };
}