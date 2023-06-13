#ifndef __RAINDROP_CORE_SCENE_COMPONENT_MANAGER_HPP__
#define __RAINDROP_CORE_SCENE_COMPONENT_MANAGER_HPP__

#include <Raindrop/Core/Scene/common.hpp>

namespace Raindrop::Core::Scene{
	class ComponentManager{
		public:
			ComponentManager(uint32_t componentSize, size_t typeID, uint32_t size, ConstructorPtr constructor, DestructorPtr destructor);
			~ComponentManager();

			void* get(ComponentHandleID id);
			uint32_t size() const;

			ComponentHandleID createComponent();
			void destroyComponent(ComponentHandleID id);

			void* array();

			void addEntity(EntityID entity);
			void removeEntity(EntityID entity);
			std::list<EntityID>& entities();
			
			ConstructorPtr constructor();
			DestructorPtr destructor();

		private:
			// create a vector of 1byte data type.
			std::vector<char> _components;
			std::queue<ComponentHandleID> _IDsPool;
			std::list<EntityID> _entities;
			
			ConstructorPtr _constructor;
			DestructorPtr _destructor;
			
			uint32_t _componentSize;
			uint32_t _size;
			size_t _typeID;
	};
}

#endif