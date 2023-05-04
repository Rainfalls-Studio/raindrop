#ifndef __RAINDROP_APPLICATION_HPP__
#define __RAINDROP_APPLICATION_HPP__

#include <common.hpp>
#include <Managers/SceneManager.hpp>
#include <Managers/AssetManager.hpp>
#include <Wrappers/wrappers.hpp>

namespace Raindrop{
	class RAINDROP_API Application{
		public:
			Application(const char* name, Version version);
			~Application();

			const char* name() const;
			Version version() const;

			Scene createScene(usize capacity);
			void destroySceen(Scene scene);
		
		private:
			std::string _name;
			Version _version;

			std::unique_ptr<Managers::SceneManager> _sceneManager;
			std::unique_ptr<Managers::AssetManager> _assetManager;
	};
}

#endif