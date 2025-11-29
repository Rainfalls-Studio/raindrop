#pragma once

#include <cstdint>
#include <expected>
#include <Raindrop/Error.hpp>

namespace Raindrop::Asset{
	class Asset{
		public:
			virtual ~Asset() = default;

            virtual std::expected<void, Error> load() = 0;
            virtual std::expected<void, Error> reload() = 0;
            virtual std::expected<void, Error> unload() = 0;

            virtual bool isLoaded() = 0;

            virtual std::uint32_t getMemoryUsage() const = 0;
	};
}