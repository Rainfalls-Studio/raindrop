#pragma once

#include <memory>
#include <string>
#include <filesystem>

namespace Raindrop::Asset{
	class Asset;

	class Factory{
		public:
			virtual ~Factory() = default;
			
			// get or create an asset based of the given parameters
			// NOTE : this function should NOT load assets but only create a valid class to reference them
			virtual std::shared_ptr<Asset> getOrCreate(const std::filesystem::path& path) = 0;

			virtual std::string getName() const noexcept = 0;
	};
}