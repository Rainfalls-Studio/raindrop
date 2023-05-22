#ifndef __RAINDROP_CORE_SCENE_ENTITY_HPP__
#define __RAINDROP_CORE_SCENE_ENTITY_HPP__

#include <Raindrop/Core/Scene/common.hpp>
#include <Raindrop/Core/Scene/Components.hpp>

namespace Raindrop::Core::Scene{
	class Entity{
		public:
			class Iterator{
				public:
					using iterator_category = std::forward_iterator_tag;
					using value_type = std::list<std::unique_ptr<Entity>>::iterator;
					using difference_type = std::ptrdiff_t;
					using pointer = std::list<std::unique_ptr<Entity>>::iterator*;
					using reference = std::list<std::unique_ptr<Entity>>::iterator&;

					Iterator(value_type it) : _it{it}{}
				
					Iterator operator++() {return Iterator(_it++);}
					Entity& operator*() {return *(_it->get());}
					bool operator==(const Iterator& other) {return _it == other._it;}
					bool operator!=(const Iterator& other) {return _it != other._it;}
					Entity* operator->(){return _it->get();}

				private:
					value_type _it;
			};

			Entity();
			~Entity();

			const std::list<std::unique_ptr<Entity>>& children() const;
			std::list<std::unique_ptr<Entity>>& children();

			Iterator begin();
			Iterator end();

			Entity& createChild();

			Components::Tag tag;
			Components::Transform transform;
		
		private:
			std::list<std::unique_ptr<Entity>> _children;
	};
}

#endif