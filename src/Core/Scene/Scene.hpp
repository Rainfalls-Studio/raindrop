#ifndef __RAINDROP_CORE_SCENE_SCENE_HPP__
#define __RAINDROP_CORE_SCENE_SCENE_HPP__

#include "../core.hpp"
#include "../common.hpp"
#include "../Memory/allocators/Allocator.hpp"
#include "../Memory/containers/Array.hpp"
#include "../Memory/containers/List.hpp"
#include "ComponentManager.hpp"
#include "EntityManager.hpp"
#include "SignatureManager.hpp"
#include "SystemManager.hpp"
#include <typeinfo>

namespace Raindrop::Core::Scene{
	static constexpr uint32 MAX_COMPONENT_COUNT = 64;

	class Scene{
		public:
			Scene(Memory::Allocator& allocator, usize capacity);
			~Scene();

			ID32 createEntity();
			void destroyEntity(ID32 entity);

			void registerComponent(usize typeId, usize typeSize, usize typeAlignement);
			void removeComponent(usize typeId);

			void* getComponent(ID32 entity, usize typeId);
			void setComponent(ID32 entity, usize typeId, void* component);
			bool hasComponent(ID32 entity, usize componentTypeId) const;
			void addComponent(ID32 entity, usize componentTypeId, void* component);
			void eraseComponent(ID32 entity, usize componentTypeId);

			void pushSystem(System* system, Signature signature);
			void popSystem(System* system);

			Signature getComponentSignature(usize typeId) const;

			template<typename T>
			Signature getComponentSignature() const{
				return getComponentSignature(typeid(T).hash_code());
			}

			template<typename... Args>
			Signature getComponentsSignature(){
				Signature signature;
				_getComponentsSignature<Args...>(signature);
				return signature;
			}

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


			template<typename T>
			bool hasComponent(ID32 entity) const{
				return hasComponent(entity, typeid(T).hash_code());
			}

			template<typename T>
			void addComponent(ID32 entity, T&& component){
				addComponent(entity, typeid(T).hash_code(), &component);
			}

			template<typename T>
			void addComponent(ID32 entity){
				addComponent(entity, typeid(T).hash_code(), nullptr);
			}

			template<typename T>
			void eraseComponent(ID32 entity){
				eraseComponent(entity, typeid(T).hash_code());
			}

			template<typename T, typename... Args>
			T* createSystem(Signature signature, Args&&... args){
				T* system = static_cast<T*>(Memory::allocateNew<T>(_allocator, args...));
				if (!system) return nullptr;
				pushSystem(static_cast<System*>(system), signature);
				return system;
			}

			template<typename T>
			void destroySystem(T* t){
				RAINDROP_assert(t);
				popSystem(static_cast<System*>(t));
				Memory::deallocateDelete(_allocator, *t);
			}

		private:
			Memory::Allocator& _allocator;
			Memory::HashMap<usize, usize> _typeToID;
			Memory::List<ID32> _freeComponentIDs;

			ComponentManager* _componentManagers[MAX_COMPONENT_COUNT];
			EntityManager _entityManager;
			SignatureManager _entitySignatures;
			SystemManager _systemManager;

			uint32 _capacity;

			template<typename T, typename A, typename... Args>
			void _getComponentsSignature(Signature& signature){
				getComponentsSignature<T>(signature);
				getComponentsSignature<A, Args...>(signature);
			}

			template<typename T>
			void _getComponentsSignature(Signature& signature){
				signature |= getComponentSignature<T>();
			}
	};
}

#endif