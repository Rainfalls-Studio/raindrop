#ifndef __RAINDROP_CORE_SCENE_SYSTEM_MANAGER_HPP__
#define __RAINDROP_CORE_SCENE_SYSTEM_MANAGER_HPP__

#include <common.hpp>
#include <Core/Memory/allocators/Allocator.hpp>
#include <Core/Scene/SignatureManager.hpp>
#include <Core/Memory/containers/List.hpp>
#include <Core/Memory/containers/DoublyLinkedList.hpp>

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

	class RAINDROP_API SystemManager{
		friend class Scene;
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