#ifndef __RAINDROP_CORE_ENGINE_HPP__
#define __RAINDROP_CORE_ENGINE_HPP__

#include <Raindrop/common.hpp>
#include <Raindrop/Core/Event/EventManager.hpp>
#include <Raindrop/Core/Scene/Scene.hpp>
#include <Raindrop/Graphics/Renderer.hpp>

namespace Raindrop::Core{
	class Engine{
		public:
			Engine();
			~Engine();

			void mainloop();

			Event::EventManager& eventManager();
			Scene::Scene& scene();

		private:
			Event::EventManager _eventManager;
			Scene::Scene _scene;
			std::unique_ptr<Graphics::Renderer> _renderer;
			
			bool launched;

			void setupEvents();
	};
}

#endif