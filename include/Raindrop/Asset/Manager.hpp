#pragma once

#include "Raindrop/config.hpp"
#include <memory>
#include <filesystem>
#include <spdlog/logger.h>
#include <type_traits>

namespace Raindrop{
	class Engine;

	namespace Asset{
		class Asset;
		class Factory;

		class Manager{
			public:
				/**
				* @brief Creates a new asset Manager
				* @param context the Asset context the manager is constructed on
				*/
				Manager(Engine& engine) noexcept;
				~Manager();

				/**
				* @brief Inserts a new asset factory in the manager
				* 
				* @param typeInfo the asset type the factory is linked to
				* @param factory the factory instance
				*/
				RAINDROP_MOCK_VIRTUAL void insertFactory(const std::type_info& typeInfo, std::shared_ptr<Factory>&& factory);

				/**
				* @brief Creates a new factory
				* 
				* @tparam Asset the asset type the factory would be linked to
				* @tparam T the factory type
				* @tparam Args the factory construction argument types
				* @param args the factory construction arguments
				* @return std::shared_ptr<T> a pointer to the factory
				*/
				template<typename Asset, typename T, typename... Args>
				std::shared_ptr<T> emplaceFactory(Args&&... args);

				/**
				* @brief Get the asset or load it if not registred
				* 
				* @param typeInfo the type of the asset
				* @param path the path of the file to load
				* @return std::shared_ptr<Asset> the asset
				*/
				RAINDROP_MOCK_VIRTUAL std::shared_ptr<Asset> loadOrGet(const std::type_info& typeInfo, const std::filesystem::path& path);

				/**
				* @brief Get the asset or load it if no registred
				* 
				* @param factoryName the name of the factory to load the asset from
				* @param path the path of the file to load
				* @return std::shared_ptr<Asset> the asset
				*/
				RAINDROP_MOCK_VIRTUAL std::shared_ptr<Asset> loadOrGet(const std::string& factoryName, const std::filesystem::path& path);

				/**
				* @brief Get the asset or load it if not registred
				* 
				* @tparam T the asset type
				* @param path the path to the asset file
				* @return std::shared_ptr<T> the asset
				*/
				template<typename T>
				std::shared_ptr<T> loadOrGet(const std::filesystem::path& path);

				// template<typename T>
				RAINDROP_MOCK_VIRTUAL std::shared_ptr<Factory> getFactory(const std::type_info& typeInfo);

				template<typename T>
				inline std::shared_ptr<T> getFactory();

				RAINDROP_MOCK_VIRTUAL std::shared_ptr<Factory> getFactory(const std::string& name);

			protected:
				Engine& _engine;
				std::shared_ptr<spdlog::logger> _logger;
				mutable std::mutex _mutex;
				
				std::unordered_map<std::string, std::size_t> _nameToFactoryID;
				std::unordered_map<std::size_t, std::weak_ptr<Factory>> _factories;
				
				void _createLogger();
				std::shared_ptr<Factory> _getFactory(std::size_t typeID);
				std::shared_ptr<Asset> _loadOrGet(std::shared_ptr<Factory> factory, const std::filesystem::path& path);
		};
	}
}

#include "inl/Manager.inl"