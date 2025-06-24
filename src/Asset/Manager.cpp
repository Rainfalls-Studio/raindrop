#include "Raindrop/Asset/Manager.hpp"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"
#include <exception>
#include <stdexcept>


namespace Raindrop::Asset{
	Manager::Manager(Engine& engine) noexcept : _engine{engine}, _factories{}
	{
		_createLogger();

		SPDLOG_LOGGER_INFO(_logger, "Manager successfully created !");
	}

	Manager::~Manager(){}

	void Manager::_createLogger(){
		_logger = spdlog::get("Core::Backend::Asset");
		if (!_logger){
			_logger = spdlog::stdout_color_mt("Core::Backend::Asset");
		}
	}

	void Manager::insertFactory(const std::type_info& typeInfo, std::shared_ptr<Factory>&& factory){
		const auto typeID = typeInfo.hash_code();
		if (_factories.count(typeID) != 0){
			SPDLOG_LOGGER_WARN(_logger, "Overwriting asset factory for {}", typeInfo.name());
		}

		_factories[typeID] = factory;
	}

	std::shared_ptr<Asset> Manager::loadOrGet(const std::type_info& typeInfo, const std::filesystem::path& path){
		auto factory = getFactory(typeInfo);
		return _loadOrGet(factory, path);
	}

	std::shared_ptr<Asset> Manager::loadOrGet(const std::string& factoryName, const std::filesystem::path& path){
		auto factory = getFactory(factoryName);
		return _loadOrGet(factory, path);
	}

	std::shared_ptr<Asset> Manager::_loadOrGet(std::shared_ptr<Factory> factory, const std::filesystem::path& path){
		return factory->getOrCreate(path);
	}

	std::shared_ptr<Factory> Manager::getFactory(const std::type_info& typeInfo){
		return _getFactory(typeInfo.hash_code());
	}

	std::shared_ptr<Factory> Manager:: getFactory(const std::string& name){
		auto it = _nameToFactoryID.find(name);
		
		if (it == _nameToFactoryID.end()){
			SPDLOG_LOGGER_ERROR(_logger, "There are no factory registred as \"{}\"", name);
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

	std::shared_ptr<Factory> Manager::_getFactory(std::size_t typeID){
		auto it = _factories.find(typeID);

		if (it == _factories.end()){
			SPDLOG_LOGGER_ERROR(_logger, "There are no factory registred for asset type \"{}\"", typeID);
			throw std::runtime_error("Factory not found");
		}

		auto factory = it->second.lock();

		if (factory == nullptr){
			_factories.erase(it);

			SPDLOG_LOGGER_ERROR(_logger, "The factory is no longer valid");
			throw std::out_of_range("Factory no longer valid");
		}

		return factory;
	}
}