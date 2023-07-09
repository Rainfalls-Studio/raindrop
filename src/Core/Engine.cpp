#include <Raindrop/Core/Engine.hpp>
#include <Raindrop/Core/Scene/Entity.hpp>


#include <Raindrop/Core/Scene/Components/Transform.hpp>
#include <Raindrop/Core/Scene/Components/Tag.hpp>
#include <Raindrop/Core/Scene/Components/Hierarchy.hpp>

INITIALIZE_EASYLOGGINGPP

namespace Raindrop::Core{
	Engine::Engine(){
		el::Logger* customLogger = el::Loggers::getLogger("Engine");
		customLogger->configurations()->set(el::Level::Global, el::ConfigurationType::Format, "%datetime %level [%logger]: %msg");

		CLOG(INFO, "Engine") << "Creating New Engine ...";

		_context = std::make_unique<EngineContext>();
		_scene = std::make_unique<Core::Scene::Scene>(*_context, 5000, 100);
		_renderer = std::make_unique<Graphics::Renderer>(*_context, *_scene);

		setupEvents();
		_context->eventManager.subscribe("Engine.Window.Quit", [&](){
			launched = false;
		});

		CLOG(INFO, "Engine") << "Engine created with success !";
	}

	Engine::~Engine(){
		CLOG(INFO, "Engine") << "Destroying Engine ...";
		_context.reset();
		_renderer.reset();
		CLOG(INFO, "Engine") << "Engine destroyed with success !";
	}

	Event::EventManager& Engine::eventManager(){
		return _context->eventManager;
	}

	Event::KeyEvents& Engine::keyEvents(){
		return _context->eventManager.keyEvents();
	}

	Event::MouseEvents& Engine::mouseEvents(){
		return _context->eventManager.mouseEvents();
	}

	Registry::Registry& Engine::registry(){
		return _context->registry;
	}

	Scene::Scene& Engine::scene(){
		return *_scene;
	}

	Asset::AssetManager& Engine::assetManager(){
		return _context->assetManager;
	}
	
	void Engine::mainloop(){
		CLOG(INFO, "Engine") << "Starting engine mainloop";
		launched = true;

		_context->eventManager.trigger("Engine.Mainloop.Start");
		while (launched){
			_context->eventManager.trigger("Engine.Mainloop.Tick");

			// {
			// 	auto& list = _scene->componentEntities<Scene::Components::DB_KeyboadController>();
			// 	if (!list.empty()){
			// 		auto entity = Scene::Entity(list.front(), _scene.get());
			// 		auto& component = entity.getComponent<Core::Scene::Components::DB_KeyboadController>();
			// 		component.update(entity.getComponent<Core::Scene::Components::Transform>(), *_eventManager);
			// 	}
			// }

			_renderer->update();
		}
		_context->eventManager.trigger("Engine.Mainloop.End");
	}

	void Engine::setupEvents(){
		_context->eventManager.subscribe("Engine.Quit", [&](){
			launched = false;
		});
	}

	Graphics::Renderer& Engine::renderer(){
		return *_renderer;
	}
}