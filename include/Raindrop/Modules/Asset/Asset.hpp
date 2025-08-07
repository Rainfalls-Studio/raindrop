#pragma once

#include <cstdint>

namespace Raindrop::Asset{
	class Asset{
		public:
			virtual ~Asset() = default;

            virtual void load() = 0;

            virtual void reload() = 0;

            virtual void unload() = 0;

            virtual std::uint32_t getMemoryUsage() const = 0;
	};
}