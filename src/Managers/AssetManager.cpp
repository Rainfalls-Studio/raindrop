#include <Managers/AssetManager.hpp>

namespace Raindrop::Managers{
	AssetManager::AssetManager(){}
	AssetManager::~AssetManager(){}

	std::shared_ptr<AssetFactory> AssetManager::_getFactory(const std::filesystem::path& path) const{
		auto it = _extensionToFactoryMap.find(path.extension());
		if (it == _extensionToFactoryMap.end()) throw std::out_of_range("this extension is not linked to any factory");
		return it->second;
	}

	bool AssetManager::exists(const std::filesystem::path& path){
		return _pathToAssetsMap.find(path) != _pathToAssetsMap.end();
	}
}