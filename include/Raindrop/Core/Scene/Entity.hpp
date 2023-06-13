#ifndef __RAINDROP_CORE_SCENE_ENTITY_HPP__
#define __RAINDROP_CORE_SCENE_ENTITY_HPP__

#include <Raindrop/Core/Scene/common.hpp>
#include <Raindrop/Core/Scene/Component.hpp>
#include <Raindrop/Core/Scene/Scene.hpp>

namespace Raindrop::Core::Scene{
	class Entity{
		public:
			class Iterator{
				public:
					using iterator_category = std::forward_iterator_tag;
					using value_type = std::list<EntityID>::iterator;
					using difference_type = std::ptrdiff_t;
					using pointer = std::list<std::unique_ptr<Entity>>::iterator*;
					using reference = std::list<std::unique_ptr<Entity>>::iterator&;

					Iterator(value_type it, Scene* scene) : _it{it}, _scene{scene}{}
				
					Iterator operator++() {return Iterator(_it++, _scene);}
					Entity operator*() {return Entity(*_it, _scene);}
					bool operator==(const Iterator& other) {return _it == other._it;}
					bool operator!=(const Iterator& other) {return _it != other._it;}
					Entity operator->(){return Entity(*_it, _scene);}

				private:
					Scene* _scene;
					value_type _it;
			};

			Entity(EntityID id, Scene* scene) : _id{id}, _scene{scene}{}
			Entity(const Entity &other) : _id{other._id}, _scene{other._scene}{}
			~Entity() = default;

			bool hasComponent(ComponentID component){
				return _scene->hasComponent(_id, component);
			}

			void* getComponent(ComponentID component){
				return _scene->getComponent(_id, component);
			}

			void* createComponent(ComponentID component){
				return _scene->createComponent(_id, component);
			}

			void destroyComponent(ComponentID component){
				_scene->destroyComponent(_id, component);
			}

			template<typename T>
			bool hasComponent(){
				return _scene->hasComponent<T>(_id);
			}

			template<typename T>
			T& getComponent(){
				return _scene->getComponent<T>(_id);
			}

			template<typename T>
			T& createComponent(){
				return _scene->createComponent<T>(_id);
			}

			template<typename T>
			void destroyComponent(){
				_scene->destroyComponent<T>(_id);
			}

			operator EntityID() const{
				return _id;
			}

			Components::Transform& transform(){
				return getComponent<Components::Transform>();
			}

			Components::Tag& tag(){
				return getComponent<Components::Tag>();
			}

			Components::Hierachy& hierachy(){
				return getComponent<Components::Hierachy>();
			}

			Iterator begin(){
				auto &a = hierachy();
				return Iterator(a.childs.begin(), _scene);
			}

			Iterator end(){
				return Iterator(hierachy().childs.end(), _scene);
			}

			EntityID id() const{
				return _id;
			}

			Scene* scene() const{
				return _scene;
			}

			Entity createChild(){
				Entity child = Entity(_scene->createEntity(), _scene);
				hierachy().childs.push_back(child);
				child.hierachy().parent = _id;
				return child;
			}

		private:
			EntityID _id;
			Scene* _scene;
	};
}

#endif