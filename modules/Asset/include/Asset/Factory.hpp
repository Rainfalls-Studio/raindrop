#pragma once

#include <memory>
#include <string>
#include "Raindrop/Modules/Filesystem/Path.hpp"

namespace Raindrop::Asset{
	class Asset;

	class Factory{
		public:
			virtual ~Factory() = default;
			
			// get or create an asset based of the given parameters
			// NOTE : this function should NOT load assets but only create a valid class to reference them
			virtual std::shared_ptr<Asset> getOrCreate(const Filesystem::Path& path) = 0;

			virtual std::string getName() const noexcept = 0;
	};
}