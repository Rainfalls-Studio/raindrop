#pragma once

#include "Resource.hpp"

namespace Raindrop::Store{
    template<typename T>
    class WriteLease {
        public:
            WriteLease() = default;
            WriteLease(std::shared_ptr<Resource<T>> res, size_t idx)
                : resource(std::move(res)), index(idx) {}

            WriteLease(const WriteLease&) = delete;
            WriteLease& operator=(const WriteLease&) = delete;

            WriteLease(WriteLease&& other) noexcept { *this = std::move(other); }
            WriteLease& operator=(WriteLease&& other) noexcept {
                if (this != &other) {
                    resource = std::move(other.resource);
                    index = other.index;
                    other.index = -1;
                }
                return *this;
            }

            ~WriteLease() {
                if (resource && index != static_cast<size_t>(-1)) {
                    resource->commit_write(index);
                }
            }

            T* operator->() {
                return &get();
            }

            T& get() {
                return resource->buffers[index].data;
            }

        private:
            std::shared_ptr<Resource<T>> resource;
            size_t index = static_cast<size_t>(-1);
    };
}