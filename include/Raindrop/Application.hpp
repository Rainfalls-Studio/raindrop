#ifndef __RAINDROP_APPLICATION_HPP__
#define __RAINDROP_APPLICATION_HPP__

#include <common.hpp>
#include <Managers/SceneManager.hpp>
#include <Managers/AssetManager.hpp>
#include <Managers/ModuleManager.hpp>
#include <Wrappers/wrappers.hpp>
#include <Graphics/GraphicsPlugin.hpp>

namespace Raindrop{
	using Raindrop::Core::IO::Plugin;

	class RAINDROP_API Application{
		public:
			Application(const char* name, Version version);
			~Application();

			const char* name() const;
			Version version() const;

			Scene createScene(usize capacity);
			void destroySceen(Scene scene);

			Module loadOrGetModule(const std::filesystem::path& path);
			bool isModuleLoaded(const std::filesystem::path& path);
			void unloadModule(const std::filesystem::path& path);

			template<typename T=Plugin, typename... Args>
			std::shared_ptr<T> createPlugin(const char* name, Args&&... args){
				return _moduleManager->createPlugin<T, Args...>(name, args...);
			}
		
		private:
			std::string _name;
			Version _version;

			std::unique_ptr<Managers::SceneManager> _sceneManager;
			std::unique_ptr<Managers::AssetManager> _assetManager;
			std::unique_ptr<Managers::ModuleManager> _moduleManager;

			void registerDefaultFactories();
	};
}

#endif