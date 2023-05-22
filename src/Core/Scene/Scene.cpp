#include <Raindrop/Core/Scene/Scene.hpp>
#include <Raindrop/Core/Scene/Entity.hpp>

namespace Raindrop::Core::Scene{
	Scene::Scene(){
		_root = std::make_unique<Entity>();
	}

	Scene::~Scene(){

	}

	Entity& Scene::root(){
		return *_root.get();
	}

	void drawChilds(Entity& e){
		printf("-%s:\n", e.tag.c_str());
		for (auto &c : e){
			drawChilds(c);
		}
	}

	void Scene::tree(){
		drawChilds(root());
	}
}