#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>

namespace ResourceRegistry{
    struct RRID{
        uint32_t id;
        uint32_t generation;

        inline size_t hash() const noexcept{
            // 64 biut pack as [id|generation]
            return (static_cast<size_t>(id) << 32) ^ static_cast<size_t>(generation);
        }

        inline bool operator==(const RRID& other) const noexcept{
            return id == other.id && generation == other.generation;
        }

        inline bool operator!=(const RRID& other) const noexcept{
            return !(*this == other);
        }
    };
}

namespace std{
    template<>
    struct hash<ResourceRegistry::RRID>{
        std::size_t operator()(const ResourceRegistry::RRID& id) const noexcept{
            return id.hash();
        }
    };
}