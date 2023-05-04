#ifndef __RAINDROP_CORE_SCENE_SYSTEM_MANAGER_HPP__
#define __RAINDROP_CORE_SCENE_SYSTEM_MANAGER_HPP__

#include <Core/Scene/common.hpp>
#include <Core/Scene/SignatureManager.hpp>
#include <Core/Memory/memory.hpp>

namespace Raindrop::Core::Scene{
	class System{
		friend class SystemManager;
		public:
			System() = default;
			virtual ~System() = default;

			virtual void onEntityCreated(EntityID entity){}
			virtual void onEntityDestroyed(EntityID entity){}
			
		protected:
			std::list<EntityID> _entities;
	};

	class RAINDROP_API SystemManager{
		friend class Scene;
		public:
			SystemManager();
			~SystemManager();

			void pushSystem(std::shared_ptr<System> system, Signature signature);
			void popSystem(std::shared_ptr<System> system);

			void entitySignatureUpdate(EntityID entity, Signature oldSignature, Signature signature);
			void entityRemoved(EntityID entity, Signature signature);

		private:
			struct SystemData{
				std::shared_ptr<System> _system;
				Signature _signature;

				bool operator==(const std::shared_ptr<System>& other){
					return _system == other;
				}
			};

			std::list<SystemData> _systems;

			void removeEntity(System* system, EntityID entity);
	};
}

#endif