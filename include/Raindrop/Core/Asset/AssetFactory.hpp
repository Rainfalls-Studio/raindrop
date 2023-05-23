#ifndef __RAINDROP_CORE_ASSET_ASSET_FACTORY_HPP__
#define __RAINDROP_CORE_ASSET_ASSET_FACTORY_HPP__

#include <Raindrop/Core/Asset/common.hpp>

namespace Raindrop::Core::Asset{
	class AssetFactory{
		public:
			AssetFactory() = default;
			virtual ~AssetFactory() = default;

			virtual std::shared_ptr<Asset> createAsset(const std::filesystem::path& path) = 0;
			virtual void destroyAsset(std::shared_ptr<Asset> asset) = 0;
	};
}

#endif