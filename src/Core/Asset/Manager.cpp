#include <Raindrop/Core/Asset/Manager.hpp>
#include <Raindrop/Core/Asset/Loader.hpp>
#include <cassert>

#include <spdlog/spdlog.h>

namespace Raindrop::Core::Asset{
	Manager::Manager(){
		spdlog::info("Creating asset manager...");
	}
	Manager::~Manager(){
		spdlog::info("Destructing asset manager...");
	}

	std::weak_ptr<Asset> Manager::get(const std::string& type, const Path& path){
		spdlog::info("Requirering asset : ({}) : {}", type, path.c_str());

		auto it = _pathToAsset.find(type);

		if (it != _pathToAsset.end()){
			return it->second;
		}

		auto loader = findLoader(type);
		assert(loader != nullptr && "The asset type is not linked to any asset loader");

		std::shared_ptr<Asset> asset = loader->load(path);
		_pathToAsset.emplace(path, asset);

		return asset;
	}

	void Manager::registerLoader(const std::string& type, const std::shared_ptr<Loader>& loader){
		spdlog::trace("Registering asset loader ({})", type);
		assert(findLoader(type) == nullptr && "The asset type is already linked with a loader");
		_typeToLoader.emplace(type, loader);
	}

	std::shared_ptr<Loader> Manager::findLoader(const std::string& type) const noexcept{
		auto it = _typeToLoader.find(type);
		if (it == _typeToLoader.end()) return nullptr;
		return it->second;
	}
}