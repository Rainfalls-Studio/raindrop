#ifndef __RAINDROP_CORE_ASSET_ASSET_MANAGER_HPP__
#define __RAINDROP_CORE_ASSET_ASSET_MANAGER_HPP__

#include <Raindrop/Core/Asset/common.hpp>

namespace Raindrop::Core::Asset{
	class AssetManager{
		public:
			AssetManager();
			~AssetManager();

			void linkFactory(const std::filesystem::path& extension, std::shared_ptr<AssetFactory> factory);
			std::shared_ptr<Asset> loadOrGet(const std::filesystem::path& path);

			template<typename T>
			std::shared_ptr<T> loadOrGet(const std::filesystem::path& path){
				return std::static_pointer_cast<T>(loadOrGet(path));
			}

		private:
			std::unordered_map<std::filesystem::path, std::shared_ptr<AssetFactory>> _extensionToFactory;
			std::unordered_map<std::filesystem::path, std::shared_ptr<Asset>> _pathToAsset;

			std::shared_ptr<AssetFactory> findFactory(const std::filesystem::path& extension);
	};
}

#endif