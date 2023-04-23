#ifndef __RAINDROP_MANAGERS_ASSET_MANAGER_HPP__
#define __RAINDROP_MANAGERS_ASSET_MANAGER_HPP__

#include <Managers/common.hpp>
#include <Managers/AssetFactory.hpp>

namespace Raindrop::Managers{
	class AssetManager{
		public:
			AssetManager(Raindrop::Core::Memory::Allocator& allocator = RAINDROP_default_allocator);
			~AssetManager();

			template<typename T>
			T& getOrCreate(Core::Memory::Allocator& allocator, const char* path){
				AssetPtr asset = _getOrCreate(allocator, path);
				return *static_cast<T*>(asset);
			}

			bool exists(const char* path);

		private:
			Core::Memory::Allocator& _allocator;
			Core::Memory::HashMap<const char*, AssetFactory*> _extensionToFactoryMap{_allocator};
			Core::Memory::HashMap<const char*, AssetPtr> _pathToAssetsMap{_allocator};

			AssetPtr _getOrCreate(Core::Memory::Allocator& allocator, const char* path);
			AssetFactory* _getFactory(const char* extension) const;
			AssetFactory* _getFactoryFromPath(const char* extension) const;

	};
}

#endif