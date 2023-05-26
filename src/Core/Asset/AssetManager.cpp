#include <Raindrop/Core/Asset/AssetManager.hpp>
#include <Raindrop/Core/Asset/AssetFactory.hpp>

namespace Raindrop::Core::Asset{
	AssetManager::AssetManager(){
		el::Logger* customLogger = el::Loggers::getLogger("AssetManager");
		customLogger->configurations()->set(el::Level::Global, el::ConfigurationType::Format, "%datetime %level [%logger]: %msg");
		CLOG(INFO, "AssetManager") << "New Asset Manager";
	}

	AssetManager::~AssetManager(){
		CLOG(INFO, "AssetManager") << "Destroying Asset Manager";
	}

	void AssetManager::linkFactory(const std::filesystem::path& extension, std::shared_ptr<AssetFactory> factory){
		_extensionToFactory[extension] = factory;
	}

	void AssetManager::destroyLink(const std::filesystem::path& extension){
		_extensionToFactory.erase(extension);
	}

	std::weak_ptr<Asset> AssetManager::loadOrGet(const std::filesystem::path& path){
		CLOG(INFO, "AssetManager") << "requirering file : " << path;

		auto it = _pathToAsset.find(path);
		if (it != _pathToAsset.end()){
			if (!it->second.expired()){
				CLOG(TRACE, "AssetManager") << "Succefuly found asset file : " << path;
				return it->second;
			}
			_pathToAsset.erase(it);
		}

		AssetFactory* factory = findFactory(path.extension());
		if (!factory){
			std::stringstream err;
			err << "Failed to find the asset factory corresponding to " << path.extension() << " extension : " << path;
			throw std::runtime_error(err.str());
		}
		auto asset = factory->createAsset(path);
		_pathToAsset[path] = asset;
		return asset;
	}

	AssetFactory* AssetManager::findFactory(const std::filesystem::path& extension){
		auto it = _extensionToFactory.find(extension);
		if (it == _extensionToFactory.end()){
			return nullptr;
		}

		if (auto factory = it->second.lock()){
			return factory.get();
		}

		_extensionToFactory.erase(extension);
		return nullptr;
	}
}