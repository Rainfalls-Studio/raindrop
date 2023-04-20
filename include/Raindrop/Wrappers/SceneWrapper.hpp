#ifndef __RAINDROP_WRAPPERS_SCENE_WRAPPER_HPP__
#define __RAINDROP_WRAPPERS_SCENE_WRAPPER_HPP__

#include <Wrappers/common.hpp>
#include <Managers/common.hpp>

namespace Raindrop::Wrappers{
	class SceneWrapper{
		public:
			SceneWrapper(const SceneWrapper& other);
			SceneWrapper(Managers::ScenePtr scene);
			~SceneWrapper();

			SceneWrapper& operator=(const SceneWrapper& other);

			/**
			 * @brief create a new valid entity handle. This entity can be connected with components. A series of structures containing data and used for certains operations.
			 * @return Entity 
			 */
			EntityWrapper createEntity();

			/**
			 * @brief Destroy a valid entity handle. the entity's components are cleared and deleted.
			 * @param entity 
			 */
			void destroyEntity(EntityWrapper entity);

			/**
			 * @brief Returns the maximal amount of entities the scene can support.
			 * @return usize 
			 */
			usize capacity();

			/**
			 * @brief Returns a registred component identifiers. This ID can be used to access entities's components
			 * @param name the name of the component to retrive
			 * @return ComponentID 
			 */
			ComponentID getComponentID(const char* name) const;

			/**
			 * @brief Returns a component ID based on he's type name. This ID can be used to access entities's components
			 * @tparam T the component to retrive
			 * @return ComponentID 
			 */
			template<typename T>
			ComponentID getComponentID() const{
				return getComponentID(typeid(T).name());
			}

			/**
			 * @brief register a new component into the scene. This component can now be added to any entities in the scene.
			 * 
			 * @tparam T The component type to add.
			 * @param name A obsional custom name. By default the type's name.
			 */
			template<typename T>
			void registerComponent(const char* name = typeid(T).name()){
				_registerComponent(name, sizeof(T), alignof(T));
			}

			/**
			 * @brief remove the component from the scene. This action also remove the component from all entities in the scene.
			 * 
			 * @tparam T The type of the component to remove.
			 * @param name A obsional custom name. By default the type's name. 
			 */
			template<typename T>
			void removeComponent(const char* name = typeid(T).name()){
				_removeComponent(name);
			}

			/**
			 * @brief returns the internal scene pointer. Can be used for debug and/or for more advanced proximity with the engine
			 * @return Managers::ScenePtr 
			 */
			Managers::ScenePtr getScenePtr() const;

			/**
			 * @brief Returns the signature of a component
			 * @return Signature 
			 */
			template<typename T>
			Signature getComponentSignature(const char* name = typeid(T).name()) const{
				return _getComponentSignature(name);
			}

			template<typename... ComponentsNames, typename... ComponentsTypes>
			Signature getComponentsSignature(ComponentsNames... names){
				Signature sig;
				const char* names_str[] = {names...};
				for (uint32 i=0; i<sizeof...(ComponentsNames); i++){
					sig |= getComponentSignature(names_str[i]);
				}
				_getComponentsSignature<ComponentsTypes...>(sig);
				return sig;
			}

		private:
			Managers::ScenePtr _scene;

			void _registerComponent(const char* name, usize size, usize alignement);
			void _removeComponent(const char* name);
			Signature _getComponentSignature(const char* name) const;

			template<typename T, typename U, typename... Args>
			void _getComponentsSignature(Signature& sig){
				_getComponentsSignature<T>(sig);
				_getComponentsSignature<U, Args...>(sig);
			}

			template<typename T>
			void _getComponentsSignature(Signature& sig){
				sig |= getComponentSignature<T>();
			}
	};
}

#endif