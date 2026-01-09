#pragma once

#include <cstdint>
#include <expected>
#include <Raindrop/Error.hpp>

namespace Asset{
	class Asset{
		public:
			virtual ~Asset() = default;

            virtual std::expected<void, Raindrop::Error> load() = 0;
            virtual std::expected<void, Raindrop::Error> reload() = 0;
            virtual std::expected<void, Raindrop::Error> unload() = 0;

            virtual bool isLoaded() = 0;

            virtual std::uint32_t getMemoryUsage() const = 0;
	};
}