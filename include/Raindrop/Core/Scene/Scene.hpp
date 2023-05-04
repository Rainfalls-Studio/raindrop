#ifndef __RAINDROP_CORE_SCENE_SCENE_HPP__
#define __RAINDROP_CORE_SCENE_SCENE_HPP__

#include <Core/Scene/common.hpp>
#include <Core/Scene/SystemManager.hpp>
#include <Core/Scene/ComponentManager.hpp>
#include <Core/Scene/EntityManager.hpp>

namespace Raindrop::Core::Scene{
	class RAINDROP_API Scene{
		public:
			Scene(usize capacity);
			~Scene();

			EntityID createEntity();
			void destroyEntity(EntityID entity);

			void registerComponent(const char* name, usize typeSize, usize typeAlignement);
			void removeComponent(const char* name);

			void* getComponent(EntityID entity, ID32 componentID);
			void setComponent(EntityID entity, ID32 componentID, void* component);
			bool hasComponent(EntityID entity, ID32 componentID) const;
			void addComponent(EntityID entity, ID32 componentID, void* component);
			void eraseComponent(EntityID entity, ID32 componentID);

			void pushSystem(std::shared_ptr<System> system, Signature signature);
			void popSystem(std::shared_ptr<System> system);

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
			T& getComponent(EntityID entity){
				return *reinterpret_cast<T*>(getComponent(entity, getComponentID(typeid(T).name())));
			}

			template<typename T>
			void setComponent(EntityID entity, T&& t){
				setComponent(entity, getComponentID(typeid(T).name()), static_cast<void*>(&t));
			}


			template<typename T>
			bool hasComponent(EntityID entity) const{
				return hasComponent(entity, getComponentID(typeid(T).name()));
			}

			template<typename T>
			void addComponent(EntityID entity, T&& component){
				addComponent(entity, getComponentID(typeid(T).name()), &component);
			}

			template<typename T>
			void addComponent(EntityID entity){
				addComponent(entity, getComponentID(typeid(T).name()), nullptr);
			}

			template<typename T>
			void eraseComponent(EntityID entity){
				eraseComponent(entity, getComponentID(typeid(T).name()));
			}

			template<typename T, typename... Args>
			std::shared_ptr<T> createSystem(Signature signature, Args&&... args){
				std::shared_ptr<T> system = std::make_shared<T>(args...);
				pushSystem(system, signature);
				return system;
			}

			template<typename T>
			void destroySystem(std::shared_ptr<T> t){
				_systemManager.popSystem(t);
			}

		private:
			std::unordered_map<const char*, ID32> _nameToID;
			std::queue<ID32> _freeComponentIDs;

			std::unique_ptr<ComponentManager> _componentManagers[MAX_COMPONENT_COUNT];
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
	};
}

#endif