#include <Raindrop/Core/Scene/Entity.hpp>

namespace Raindrop::Core::Scene{
	Entity::Entity(){

	}

	Entity::~Entity(){

	}

	const std::list<std::unique_ptr<Entity>>& Entity::children() const{
		return _children;
	}
	
	std::list<std::unique_ptr<Entity>>& Entity::children(){
		return _children;
	}

	Entity::Iterator Entity::begin(){
		return Iterator(_children.begin());
	}

	Entity::Iterator Entity::end(){
		return Iterator(_children.end());
	}

	Entity& Entity::createChild(){
		_children.push_back(std::make_unique<Entity>());
		return *_children.back().get();
	}
}