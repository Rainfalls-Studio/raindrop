#include <Raindrop/Raindrop.hpp>
#include <gtest/gtest.h>

TEST(sceneWrapper, a){
	Raindrop::Core::Scene::Scene scene_ptr(RAINDROP_default_allocator, 1000);
	Raindrop::Scene scene(&scene_ptr);

	Raindrop::Entity entity = scene.createEntity();
	EXPECT_NE(entity, Raindrop::INVALID_ENTITY);
	EXPECT_TRUE(entity.exists());

	EXPECT_NO_THROW(scene.destroyEntity(entity));
}