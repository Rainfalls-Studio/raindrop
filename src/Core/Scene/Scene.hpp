#ifndef __RAINDROP_CORE_SCENE_SCENE_HPP__
#define __RAINDROP_CORE_SCENE_SCENE_HPP__

#include "../core.hpp"
#include "../common.hpp"
#include "../Memory/allocators/Allocator.hpp"
#include "../Memory/containers/Array.hpp"
#include "../Memory/containers/List.hpp"
#include "ComponentManager.hpp"
#include "EntityManager.hpp"
#include <typeinfo>

namespace Raindrop::Core::Scene{
	class Scene{
		public:
			Scene(Memory::Allocator& allocator, usize capacity, usize componentCount);
			~Scene();

			ID32 createEntity();
			void destroyEntity(ID32 entity);

			void registerComponent(usize typeId, usize typeSize, usize typeAlignement);
			void removeComponent(usize typeId);

			void* getComponent(ID32 entity, usize typeId);
			void setComponent(ID32 entity, usize typeId, void* component);

			template<typename T>
			void registerComponent(){
				registerComponent(typeid(T).hash_code(), sizeof(T), alignof(T));
			}

			template<typename T>
			void removeComponent(){
				removeComponent(typeid(T).hash_code());
			}

			template<typename T>
			T& getComponent(ID32 entity){
				return *reinterpret_cast<T*>(getComponent(entity, typeid(T).hash_code()));
			}

			template<typename T>
			void setComponent(ID32 entity, T&& t){
				setComponent(entity, typeid(T).hash_code(), static_cast<void*>(&t));
			}

		private:
			Memory::Allocator& _allocator;
			Memory::Array<ComponentManager*> _managers;
			Memory::HashMap<usize, usize> _typeToID;
			Memory::List<ID32> _freeComponentIDs;
			EntityManager _entityManager;

			uint32 _capacity;

			// void removeComponentID(ID32 id);
	};
}

#endif