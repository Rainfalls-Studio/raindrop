#pragma once

#include <memory>
#include <spdlog/logger.h>
#include <Raindrop/Modules/Filesystem/Path.hpp>
#include <Raindrop/Core/Modules/IModule.hpp>

namespace Raindrop{
	class Engine;

	namespace Asset{
		class Asset;
		class Factory;

		class AssetModule : public Modules::IModule{
			public:
				/**
				* @brief Creates a new asset Manager
				* @param context the Asset context the manager is constructed on
				*/
				AssetModule() noexcept;
				~AssetModule();


				virtual Modules::Result initialize(Modules::InitHelper& helper) override;
				virtual void shutdown() override;

            	virtual Name name() const noexcept override;

				/**
				* @brief Inserts a new asset factory in the manager
				* 
				* @param typeInfo the asset type the factory is linked to
				* @param factory the factory instance
				*/
				void insertFactory(const std::type_info& typeInfo, std::shared_ptr<Factory>&& factory);

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
				std::shared_ptr<Asset> loadOrGet(const std::type_info& typeInfo, const Filesystem::Path& path);

				/**
				* @brief Get the asset or load it if no registred
				* 
				* @param factoryName the name of the factory to load the asset from
				* @param path the path of the file to load
				* @return std::shared_ptr<Asset> the asset
				*/
				std::shared_ptr<Asset> loadOrGet(const std::string& factoryName, const Filesystem::Path& path);

				/**
				* @brief Get the asset or load it if not registred
				* 
				* @tparam T the asset type
				* @param path the path to the asset file
				* @return std::shared_ptr<T> the asset
				*/
				template<typename T>
				std::shared_ptr<T> loadOrGet(const Filesystem::Path& path);

				// template<typename T>
				std::shared_ptr<Factory> getFactory(const std::type_info& typeInfo);

				template<typename T>
				inline std::shared_ptr<T> getFactory();

				std::shared_ptr<Factory> getFactory(const std::string& name);

			protected:
				Engine* _engine;
				mutable std::mutex _mutex;
				
				std::unordered_map<std::string, std::size_t> _nameToFactoryID;
				std::unordered_map<std::size_t, std::weak_ptr<Factory>> _factories;
				
				std::shared_ptr<Factory> _getFactory(std::size_t typeID);
				std::shared_ptr<Asset> _loadOrGet(std::shared_ptr<Factory> factory, const Filesystem::Path& path);
		};
	}
}

#include "inl/AssetModule.inl"