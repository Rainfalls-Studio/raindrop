#ifndef __RAINDROP_CORE_ENGINE_HPP__
#define __RAINDROP_CORE_ENGINE_HPP__

#include <Raindrop/common.hpp>
#include <Raindrop/Core/Event/KeyEvents.hpp>
#include <Raindrop/Core/Scene/Scene.hpp>
#include <Raindrop/Graphics/Renderer.hpp>
#include <Raindrop/Core/EngineContext.hpp>

namespace Raindrop::Core{
	class Engine{
		public:
			Engine();
			~Engine();

			void mainloop();

			Event::EventManager& eventManager();
			Event::KeyEvents& keyEvents();
			Event::MouseEvents& mouseEvents();
			
			Asset::AssetManager& assetManager();
			Registry::Registry& registry();
			Scene::Scene& scene();
			Graphics::Renderer& renderer();

		private:
			std::unique_ptr<EngineContext> _context;
			std::unique_ptr<Scene::Scene> _scene;

			std::unique_ptr<Graphics::Renderer> _renderer;
			
			bool launched;

			void setupEvents();
	};
}

#endif