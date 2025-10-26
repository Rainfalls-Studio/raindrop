#include "Raindrop/Modules/Asset/AssetModule.hpp"
#include "Raindrop/Core/Modules/InitHelper.hpp"
#include "spdlog/spdlog.h"
#include <exception>
#include <stdexcept>


namespace Raindrop::Asset{
	AssetModule::AssetModule() noexcept : _factories{}{}
	AssetModule::~AssetModule(){}


	Modules::Result AssetModule::initialize(Modules::InitHelper& helper){
		_engine = &helper.engine();
		spdlog::info("AssetModule successfully created !");

		return Modules::Result::Success();
	}

	void AssetModule::shutdown(){
		_factories.clear();
		_nameToFactoryID.clear();
	}

	AssetModule::Name AssetModule::name() const noexcept{
		return "Asset";
	}

	void AssetModule::insertFactory(const std::type_info& typeInfo, std::shared_ptr<Factory>&& factory){
		const auto typeID = typeInfo.hash_code();
		if (_factories.count(typeID) != 0){
			spdlog::warn("Overwriting asset factory for {}", typeInfo.name());
		}

		_factories[typeID] = factory;
	}

	std::shared_ptr<Asset> AssetModule::loadOrGet(const std::type_info& typeInfo, const Filesystem::Path& path){
		auto factory = getFactory(typeInfo);
		return _loadOrGet(factory, path);
	}

	std::shared_ptr<Asset> AssetModule::loadOrGet(const std::string& factoryName, const Filesystem::Path& path){
		auto factory = getFactory(factoryName);
		return _loadOrGet(factory, path);
	}

	std::shared_ptr<Asset> AssetModule::_loadOrGet(std::shared_ptr<Factory> factory, const Filesystem::Path& path){
		return factory->getOrCreate(path);
	}

	std::shared_ptr<Factory> AssetModule::getFactory(const std::type_info& typeInfo){
		return _getFactory(typeInfo.hash_code());
	}

	std::shared_ptr<Factory> AssetModule:: getFactory(const std::string& name){
		auto it = _nameToFactoryID.find(name);
		
		if (it == _nameToFactoryID.end()){
			spdlog::error("There are no factory registred as \"{}\"", name);
			throw std::out_of_range("Factory not found");
		}
		
		// If it fails to find the factory, then we must remove it from the list as it's no longer valid
		try{
			return _getFactory(it->second);
		} catch (const std::exception& e){
			_nameToFactoryID.erase(it);
			throw;
		}
	}

	std::shared_ptr<Factory> AssetModule::_getFactory(std::size_t typeID){
		auto it = _factories.find(typeID);

		if (it == _factories.end()){
			spdlog::error("There are no factory registred for asset type \"{}\"", typeID);
			throw std::runtime_error("Factory not found");
		}

		auto factory = it->second.lock();

		if (factory == nullptr){
			_factories.erase(it);

			spdlog::error("The factory is no longer valid");
			throw std::out_of_range("Factory no longer valid");
		}

		return factory;
	}
}