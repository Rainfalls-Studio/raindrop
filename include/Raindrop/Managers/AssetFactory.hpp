#ifndef __RAINDROP_MANAGERS_ASSET_FACTORY_HPP__
#define __RAINDROP_MANAGERS_ASSET_FACTORY_HPP__

#include <Managers/common.hpp>

namespace Raindrop::Managers{
	class AssetFactory{
		public:
			AssetFactory() = default;
			virtual ~AssetFactory() = default;

			virtual AssetPtr create(Core::Memory::Allocator& allocator, const char* path) = 0;
			virtual void destroy(AssetPtr asset) = 0;
			virtual usize getTypeID() const = 0;

		private:

	};
}

#endif