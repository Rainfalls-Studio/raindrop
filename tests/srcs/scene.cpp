#include "test.hpp"

TEST(scene, initialization){
	Raindrop::usize memSize = MEMORY_MO * 4;
	void* ptr = malloc(memSize);

	{
		Raindrop::Core::Memory::FreelistAllocator allocator(memSize, ptr);
		Raindrop::Core::Scene::Scene scene(allocator, 1024);
	}

	free(ptr);
}

TEST(scene, registerComponents){
	Raindrop::usize memSize = MEMORY_MO * 4;
	void* ptr = malloc(memSize);

	{
		Raindrop::Core::Memory::FreelistAllocator allocator(memSize, ptr);
		Raindrop::Core::Scene::Scene scene(allocator, 1024);

		scene.registerComponent<struct32>();
		scene.registerComponent<struct128>();
		scene.registerComponent<struct8>();
	}

	free(ptr);
}

TEST(scene, createEntities){
	Raindrop::usize memSize = MEMORY_MO * 4;
	void* ptr = malloc(memSize);

	Raindrop::ID32 entities[500];

	{
		Raindrop::Core::Memory::FreelistAllocator allocator(memSize, ptr);
		Raindrop::Core::Scene::Scene scene(allocator, 1024);
		
		for (int i=0; i<500; i++){
			entities[i] = scene.createEntity();
		}

		for (int i=0; i<500; i++){
			scene.destroyEntity(entities[i]);
		}
	}

	free(ptr);
}

TEST(scene, entitySignatures){
	Raindrop::usize memSize = MEMORY_MO * 4;
	void* ptr = malloc(memSize);

	Raindrop::ID32 entities[500];

	{
		Raindrop::Core::Memory::FreelistAllocator allocator(memSize, ptr);
		Raindrop::Core::Scene::Scene scene(allocator, 1024);

		scene.registerComponent<struct32>();
		scene.registerComponent<struct128>();
		scene.registerComponent<struct8>();
		
		for (int i=0; i<500; i++){
			entities[i] = scene.createEntity();
		}

		for (int i=0; i<500; i++){
			int a = i%3;

			switch (a){
				case 0: scene.addComponent<struct32>(entities[i]);
				case 1: scene.addComponent<struct128>(entities[i]);
				case 2: scene.addComponent<struct8>(entities[i]);
			}
		}

		for (int i=0; i<500; i++){
			if (scene.hasComponent<struct32>(entities[i])){
				auto& component = scene.getComponent<struct32>(entities[i]);
				continue;
			}

			if (scene.hasComponent<struct128>(entities[i])){
				auto& component = scene.getComponent<struct128>(entities[i]);
				continue;
			}
			
			if (scene.hasComponent<struct8>(entities[i])){
				auto& component = scene.getComponent<struct8>(entities[i]);
				continue;
			}
		}

		for (int i=0; i<500; i++){
			scene.destroyEntity(entities[i]);
		}

	}

	free(ptr);
}