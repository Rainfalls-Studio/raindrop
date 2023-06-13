#ifndef __RAINDROP_CORE_SCENE_COMPONENT_REGISTRY_HPP__
#define __RAINDROP_CORE_SCENE_COMPONENT_REGISTRY_HPP__

#include <Raindrop/Core/Scene/common.hpp>

namespace Raindrop::Core::Scene{
	class ComponentRegistry{
		public:
			ComponentRegistry(uint32_t size);
			~ComponentRegistry();

			ComponentID registerComponent(uint32_t componentSize, size_t typeID, uint32_t size, ConstructorPtr constructor, DestructorPtr destructor);
			void unregisterComponent(ComponentID id);
			ComponentID getComponentID(size_t typeID);

			void* getComponent(ComponentID ID, ComponentHandleID handleID);

			uint32_t size() const;
			uint32_t usedSize() const;

			template<typename T>
			void registerComponent(uint32_t size){
				registerComponent(sizeof(T), typeid(T).hash_code(), size, [](void* component){new (component) T();}, [](void* component){reinterpret_cast<T*>(component)->~T();});
			}

			template<typename T>
			void unregisterComponent(){
				unregisterComponent(typeid(T).hash_code());
			}

			template<typename T>
			T& getComponent(ComponentHandleID id){
				return *reinterpret_cast<T*>(getComponent(typeid(T).hash_code(), id));
			}

			ComponentHandleID createHandle(ComponentID component, EntityID entity);
			void destroyHandle(ComponentID component, ComponentHandleID handle, EntityID entity);

			bool registred(size_t typeID);

			ComponentManager* getManager(ComponentID component);

		private:
			std::unordered_map<size_t, ComponentID> _typeIDtoComponentID;
			std::vector<std::unique_ptr<ComponentManager>> _managers;
			std::queue<ComponentID> _IDsPool;

			ComponentID getNextID();
	};
}

#endif