#pragma once

#include "Resource.hpp"

namespace Raindrop::Store{
    template<typename T>
    class ReadLease {
        public:
            ReadLease() = default;
            ReadLease(std::shared_ptr<Resource<T>> res, size_t idx)
                : resource(std::move(res)), index(idx) {}

            ReadLease(const ReadLease&) = delete;
            ReadLease& operator=(const ReadLease&) = delete;

            ReadLease(ReadLease&& other) noexcept {
                *this = std::move(other);
            }

            ReadLease& operator=(ReadLease&& other) noexcept {
                if (this != &other) {
                    release();
                    resource = std::move(other.resource);
                    index = other.index;
                    other.index = -1;
                }
                return *this;
            }

            ~ReadLease() {
                release();
            }

            inline T* operator->() {
                return &get();
            }

            inline const T* operator->() const {
                return &get();
            }

            T& get() {
                return resource->buffers[index].data;
            }

            const T& get() const {
                return resource->buffers[index].data;
            }

            void release() {
                if (resource && index != static_cast<size_t>(-1)) {
                    resource->release_reader(index);
                    resource.reset();
                    index = static_cast<size_t>(-1);
                }
            }

            explicit operator bool() const noexcept {
                return resource != nullptr;
            }

        private:
            std::shared_ptr<Resource<T>> resource;
            size_t index = static_cast<size_t>(-1);
    };
}