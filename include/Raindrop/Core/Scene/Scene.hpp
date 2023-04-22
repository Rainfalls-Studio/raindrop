#ifndef __RAINDROP_CORE_SCENE_SCENE_HPP__
#define __RAINDROP_CORE_SCENE_SCENE_HPP__

#include <Core/Scene/common.hpp>
#include <Core/Scene/SystemManager.hpp>
#include <Core/Scene/ComponentManager.hpp>
#include <Core/Scene/EntityManager.hpp>
#include <Core/Memory/allocators/Allocator.hpp>
#include <Core/Memory/containers/HashMap.hpp>

namespace Raindrop::Core::Scene{
	class RAINDROP_API Scene{
		public:
			Scene(Memory::Allocator& allocator, usize capacity);
			~Scene();

			ID32 createEntity();
			void destroyEntity(ID32 entity);
			bool exists(ID32 entity) const;

			void registerComponent(const char* name, usize typeSize, usize typeAlignement);
			void removeComponent(const char* name);

			void* getComponent(ID32 entity, ID32 componentID);
			void setComponent(ID32 entity, ID32 componentID, void* component);
			bool hasComponent(ID32 entity, ID32 componentID) const;
			void addComponent(ID32 entity, ID32 componentID, void* component);
			void eraseComponent(ID32 entity, ID32 componentID);

			void pushSystem(System* system, Signature signature);
			void popSystem(System* system);

			Signature getComponentSignature(ID32 componentID) const;
			ID32 getComponentID(const char* name) const;
			usize capacity() const;


			template<typename T>
			Signature getComponentSignature() const{
				return getComponentSignature(getComponentID(typeid(T).name()));
			}

			template<typename... Args>
			Signature getComponentsSignature(){
				Signature signature;
				_getComponentsSignature<Args...>(signature);
				return signature;
			}

			template<typename T>
			void registerComponent(const char* name = typeid(T).name()){
				registerComponent(name, sizeof(T), alignof(T));
			}

			template<typename T>
			void removeComponent(const char* name = typeid(T).name()){
				removeComponent(name);
			}

			template<typename T>
			T& getComponent(ID32 entity){
				return *reinterpret_cast<T*>(getComponent(entity, getComponentID(typeid(T).name())));
			}

			template<typename T>
			void setComponent(ID32 entity, T&& t){
				setComponent(entity, getComponentID(typeid(T).name()), static_cast<void*>(&t));
			}


			template<typename T>
			bool hasComponent(ID32 entity) const{
				return hasComponent(entity, getComponentID(typeid(T).name()));
			}

			template<typename T>
			void addComponent(ID32 entity, T&& component){
				addComponent(entity, getComponentID(typeid(T).name()), &component);
			}

			template<typename T>
			void addComponent(ID32 entity){
				addComponent(entity, getComponentID(typeid(T).name()), nullptr);
			}

			template<typename T>
			void eraseComponent(ID32 entity){
				eraseComponent(entity, getComponentID(typeid(T).name()));
			}

			template<typename T, typename... Args>
			T* createSystem(Signature signature, Args&&... args){
				T* system = static_cast<T*>(Memory::allocateNew<T>(_allocator, args...));
				if (!system) return nullptr;
				pushSystem(dynamic_cast<System*>(system), signature);
				return system;
			}

			template<typename T>
			void destroySystem(T& t){
				RAINDROP_assert(t);
				popSystem(&static_cast<System&>(t));
				Memory::deallocateDelete(_allocator, t);
			}

		private:
			Memory::Allocator& _allocator;
			Memory::HashMap<const char*, ID32> _nameToID;
			Memory::List<ID32> _freeComponentIDs;

			ComponentManager* _componentManagers[MAX_COMPONENT_COUNT];
			EntityManager _entityManager;
			SignatureManager _entitySignatures;
			SystemManager _systemManager;

			uint32 _capacity;

			static bool isIDValid(ID32 id);

			template<typename T, typename A, typename... Args>
			void _getComponentsSignature(Signature& signature){
				getComponentsSignature<T>(signature);
				getComponentsSignature<A, Args...>(signature);
			}

			template<typename T>
			void _getComponentsSignature(Signature& signature){
				signature |= getComponentSignature<T>();
			}

			void deleteSystems();
	};
}

#endif