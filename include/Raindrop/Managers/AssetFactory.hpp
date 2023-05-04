#ifndef __RAINDROP_MANAGERS_ASSET_FACTORY_HPP__
#define __RAINDROP_MANAGERS_ASSET_FACTORY_HPP__

#include <Managers/common.hpp>

namespace Raindrop::Managers{
	class AssetFactory{
		friend class Asset;
		public:
			AssetFactory() = default;
			virtual ~AssetFactory() = default;

			virtual std::shared_ptr<Asset> create(const std::filesystem::path& path) = 0;

		protected:
			virtual void destroy(Asset* asset) = 0;
	};
}

#endif