#ifndef __RAINDROP_WRAPPERS_SCENE_WRAPPER_HPP__
#define __RAINDROP_WRAPPERS_SCENE_WRAPPER_HPP__

#include <Wrappers/common.hpp>
#include <Managers/common.hpp>
#include <Wrappers/SystemWrapper.hpp>

namespace Raindrop::Wrappers{
	class RAINDROP_API SceneWrapper{
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

			/**
			 * @brief get the signature of all the required components
			 * 
			 * @tparam ComponentsNames
			 * @tparam ComponentsTypes the required components types. 
			 * @param names components name. Components signatures can be retrived using there name.
			 * @return Signature 
			 */
			template<typename... ComponentsTypes, typename... ComponentsNames>
			Signature getComponentsSignature(ComponentsNames... names){
				Signature sig;
				const char* names_str[] = {names...};
				for (uint32 i=0; i<sizeof...(ComponentsNames); i++){
					sig |= getComponentSignature(names_str[i]);
				}
				_getComponentsSignature<ComponentsTypes...>(sig);
				return sig;
			}

			/**
			 * @brief get the signature of all the required components
			 * 
			 * @tparam ComponentsTypes the required components types.
			 * @return Signature 
			 */
			template<typename... ComponentsTypes>
			Signature getComponentsSignature(){
				Signature sig;
				_getComponentsSignature<ComponentsTypes...>(sig);
				return sig;
			}
			
			/**
			 * @brief create a system and registers it in the scene.
			 * 
			 * @tparam T the type if the system. Has to be a derived class of SystemBase
			 * @tparam Args the required arguments of the system constructor
			 * @param signature the required components of the system. Create a signature using 'getComponentsSignature'
			 * @param args the arguments required by the system constuctore
			 * @return T& a reference to the freshly created system
			 */
			template<typename T, typename... Args>
			T& createSystem(Signature signature, Args&&... args){
				if constexpr (!std::is_base_of<SystemBase, T>::value) throw std::invalid_argument("cannot create a system with a class that is not derived from the Raindrop Base System class");

				if (auto scene = _scene.lock()){
					T* system = scene->createSystem<T, Args...>(signature, args...);
					SystemBase* base = static_cast<SystemBase*>(system);
					base->_scene = _scene;
					return *system;
				} else {
					throw std::runtime_error("the scene is expired");
				}
			}

			/**
			 * @brief create a system and registers it in the scene.
			 * 
			 * @tparam T the type if the system. Has to be a derived class of SystemBase
			 * @param signature the required components of the system. Create a signature using 'getComponentsSignature'
			 * @return T& a reference to the freshly created system
			 */
			template<typename T>
			T& createSystem(Signature signature){
				if constexpr (!std::is_base_of<SystemBase, T>::value) throw std::invalid_argument("cannot create a system with a class that is not derived from the Raindrop Base System class");

				if (auto scene = _scene.lock()){
					T* system = scene->createSystem<T>(signature);
					SystemBase* base = static_cast<SystemBase*>(system);
					base->_scene = _scene;
					return *system;
				} else {
					throw std::runtime_error("the scene is expired");
				}
			}

			template<typename T>
			void destoySystem(T& t){
				if constexpr (!std::is_base_of<SystemBase, T>::value) throw std::invalid_argument("cannot destroy a system with a class that is not derived from the Raindrop Base System class");
				if (auto scene = _scene.lock()){
					scene->destroySystem<T>(t);
				} else {
					throw std::runtime_error("the scene is expired");
				}
			}

			bool expired() const;

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