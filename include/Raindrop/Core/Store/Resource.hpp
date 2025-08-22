#pragma once

#include <memory>
#include <mutex>
#include <functional>
#include <condition_variable>

#include "Types.hpp"
#include "Raindrop/Core/Time/Clock.hpp"


namespace Raindrop::Store{
    // The core Resource class, holds multiple buffers and synchronization state
    template<typename T>
    class Resource : public std::enable_shared_from_this<Resource<T>> {
        public:
            class WriteAccess;
            class ReadAccess;

            struct Buffer {
                T data; // The actual data
                std::atomic<size_t> readers = 0; // number of active readers
                std::atomic<bool> retired = false; // if the writer ommited this resource
                std::function<void(T&)> cleanup; // called when reader count falls to zero
                bool writing = false; // protected by mutex


                Buffer() = default;
            };

            explicit Resource(size_t bufferCount) : buffers(bufferCount){}

            size_t bufferCount() const noexcept{
                return buffers.size();
            }

            // Non-blocking latest
            std::optional<ReadLease<T>> try_acquire_read_latest() {
                return try_acquire_read(latest_index.load(std::memory_order_acquire));
            }

            // Non-blocking by index
            std::optional<ReadLease<T>> try_acquire_read(size_t index) {
                if (index >= buffers.size()) return std::nullopt;
                auto& v = buffers[index];
                if (v.retired.load(std::memory_order_acquire)) return std::nullopt;
                v.readers.fetch_add(1, std::memory_order_acq_rel);
                return ReadLease<T>(this->shared_from_this(), index);
            }

            // Blocking acquire latest
            ReadLease<T> blocking_acquire_read_latest() {
                std::unique_lock<std::mutex> lk(mtx);
                size_t idx;

                while (true) {
                    idx = latest_index.load(std::memory_order_acquire);
                    auto& v = buffers[idx];
                    if (!v.retired.load(std::memory_order_acquire)) {
                        v.readers.fetch_add(1, std::memory_order_acq_rel);
                        return ReadLease<T>(this->shared_from_this(), idx);
                    }
                    cv.wait(lk); // wait for a release
                }
            }

            // Blocking acquire by index
            ReadLease<T> blocking_acquire_read(size_t index) {
                if (index >= buffers.size()) throw std::out_of_range("buffer index");
                std::unique_lock<std::mutex> lk(mtx);
                while (true) {
                    auto& v = buffers[index];
                    if (!v.retired.load(std::memory_order_acquire)) {
                        v.readers.fetch_add(1, std::memory_order_acq_rel);
                        return ReadLease<T>(this->shared_from_this(), index);
                    }
                    cv.wait(lk);
                }
            }

            WriteLease<T> blocking_acquire_write(size_t index) {
                if (index >= buffers.size()) throw std::out_of_range("buffer index");
                std::unique_lock<std::mutex> lk(mtx);
                auto& v = buffers[index];

                cv.wait(lk, [&]() {
                    // wait until no readers and no writer
                    return v.readers.load(std::memory_order_acquire) == 0 &&
                        !v.writing;
                });

                // mark writing
                v.writing = true;

                return WriteLease<T>(this->shared_from_this(), index);
            }

            // Writers always succeed (we assume single writer at a time externally)
            WriteLease<T> acquire_write(size_t index) {
                if (index >= buffers.size()) throw std::out_of_range("buffer index");
                return WriteLease<T>(this->shared_from_this(), index);
            }

            void commit_write(size_t index) {
                auto& v = buffers[index];
                v.writing = false;
                
                latest_index.store(index, std::memory_order_release);
                notify_all();
            }

            void retire_version(size_t index) {
                if (index >= buffers.size()) return;
                buffers[index].retired.store(true, std::memory_order_release);
                check_cleanup(index);
                notify_all();
            }

            void retire_all() {
                for (size_t i = 0; i < buffers.size(); ++i) {
                    retire_version(i);
                }
            }

            void notify_all() {
                std::scoped_lock lk(mtx);
                cv.notify_all();
            }

        private:
            friend class ReadLease<T>;
            friend class WriteLease<T>;

            mutable std::mutex mtx;
            mutable std::condition_variable cv;


            void release_reader(size_t index) {
                auto& v = buffers[index];
                
                if (v.readers.fetch_sub(1, std::memory_order_acq_rel) == 1) {
                    check_cleanup(index);
                    notify_all();
                }
            }

            void check_cleanup(size_t index) {
                auto& v = buffers[index];

                if (v.retired.load(std::memory_order_acquire) &&
                    v.readers.load(std::memory_order_acquire) == 0){

                    if (v.cleanup) v.cleanup(v.data);
                }
            }

            std::vector<Buffer> buffers;
            std::atomic<size_t> latest_index;
    };

    template<typename T>
    using ResourcePtr = std::shared_ptr<Resource<T>>;
}