#include <Raindrop/Core/Asset/AssetManager.hpp>
#include <Raindrop/Core/Asset/AssetFactory.hpp>

namespace Raindrop::Core::Asset{
	AssetManager::AssetManager(){}

	AssetManager::~AssetManager(){
		for (auto& s : _pathToAsset){
			auto factory = findFactory(s.first.extension());
			if (factory){
				factory->destroyAsset(s.second);
			}
		}
	}

	void AssetManager::linkFactory(const std::filesystem::path& extension, std::shared_ptr<AssetFactory> factory){
		_extensionToFactory[extension] = factory;
	}

	std::shared_ptr<Asset> AssetManager::loadOrGet(const std::filesystem::path& path){
		std::shared_ptr<AssetFactory> factory = findFactory(path.extension());
		if (!factory){
			std::stringstream err;
			err << "Failed to find the asset factory corresponding to " << path.extension() << " extension : " << path;
			throw std::runtime_error(err.str());
		}
		return factory->createAsset(path);
	}

	std::shared_ptr<AssetFactory> AssetManager::findFactory(const std::filesystem::path& extension){
		auto it = _extensionToFactory.find(extension);
		if (it == _extensionToFactory.end()){
			return nullptr;
		}
		return it->second;
	}

}