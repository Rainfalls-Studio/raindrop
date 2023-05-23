#ifndef __RAINDROP_CORE_ASSET_ASSET_HPP__
#define __RAINDROP_CORE_ASSET_ASSET_HPP__

#include <Raindrop/Core/Asset/common.hpp>

namespace Raindrop::Core::Asset{
	class Asset{
		public:
			Asset() = default;
			virtual ~Asset() = default;
	};
}

#endif