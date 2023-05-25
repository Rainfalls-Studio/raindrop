#include <Raindrop/Core/Engine.hpp>

namespace Raindrop::Core{
	Engine::Engine(){
		_renderer = std::make_unique<Graphics::Renderer>(_eventManager, _assetManager);

		setupEvents();
		_eventManager.subscribe("engine.window.quit", [&](){
			launched = false;
		});
	}

	Engine::~Engine(){

	}

	Event::EventManager& Engine::eventManager(){
		return _eventManager;
	}

	Scene::Scene& Engine::scene(){
		return _scene;
	}

	Asset::AssetManager& Engine::assetManager(){
		return _assetManager;
	}
	
	void Engine::mainloop(){
		launched = true;

		_eventManager.trigger("engine.loop.start");
		while (launched){
			_eventManager.trigger("engine.loop.tick");
			_renderer->update();
		}
		_eventManager.trigger("engine.loop.end");
	}

	void Engine::setupEvents(){
		_eventManager.subscribe("engine.quit", [&](){
			launched = false;
		});
	}
}