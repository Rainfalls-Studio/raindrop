#ifndef __RAINDROP_CORE_SCENE_SYSTEM_MANAGER_HPP__
#define __RAINDROP_CORE_SCENE_SYSTEM_MANAGER_HPP__

#include "../core.hpp"
#include "../common.hpp"
#include "../Memory/allocators/Allocator.hpp"
#include "SignatureManager.hpp"
#include "../Memory/containers/List.hpp"
#include "../Memory/containers/DoublyLinkedList.hpp"

namespace Raindrop::Core::Scene{
	class System{
		friend class SystemManager;
		public:
			System(Memory::Allocator& allocator) : _allocator{allocator}, _entities{allocator}{}
			virtual ~System() = default;

			virtual void onEntityCreated(ID32 entity){}
			virtual void onEntityDestroyed(ID32 entity){}
			
		protected:
			Memory::Allocator& _allocator;
			Memory::DoublyLinkedList<ID32> _entities;
	};

	class SystemManager{
		public:
			SystemManager(Memory::Allocator& allocator);
			~SystemManager();

			void pushSystem(System* system, Signature signature);
			void popSystem(System* system);

			void entitySignatureUpdate(ID32 entity, Signature oldSignature, Signature signature);
			void entityRemoved(ID32 entity, Signature signature);

		private:
			struct SystemData{
				System* _system;
				Signature _signature;
			};

			Memory::List<SystemData> _systems;

			void removeEntity(System* system, ID32 entity);
	};
}

#endif