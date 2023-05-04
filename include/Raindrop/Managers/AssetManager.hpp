#ifndef __RAINDROP_MANAGERS_ASSET_MANAGER_HPP__
#define __RAINDROP_MANAGERS_ASSET_MANAGER_HPP__

#include <Managers/common.hpp>
#include <Managers/AssetFactory.hpp>

namespace Raindrop::Managers{
	class AssetManager{
		public:
			AssetManager();
			~AssetManager();

			template<typename T>
			std::weak_ptr<T> getOrCreate(const std::filesystem::path& path){
				{
					auto it = _pathToAssetsMap.find(path);
					if (it != _pathToAssetsMap.end()) return std::dynamic_pointer_cast<T>(*it);
				}

				std::shared_ptr<Asset> asset = _getFactory(path)->create(path);
				_pathToAssetsMap[path] = asset;

				return asset;
			}

			bool exists(const std::filesystem::path& path);

		private:
			std::unordered_map<std::filesystem::path, std::shared_ptr<AssetFactory>> _extensionToFactoryMap;
			std::unordered_map<std::filesystem::path, std::shared_ptr<Asset>> _pathToAssetsMap;

			std::shared_ptr<AssetFactory> _getFactory(const std::filesystem::path& path) const;

	};
}

#endif