#include <Raindrop/Raindrop.hpp>
#include <gtest/gtest.h>

TEST(application, valid_initialization){
	EXPECT_NO_THROW(Raindrop::Application app("app", RAINDROP_CreateVersion(1, 0, 0)));
}

TEST(application, create_valid_scene){
	Raindrop::Application app("app", RAINDROP_CreateVersion(1, 0, 0));
	
	Raindrop::ScenePtr scene = Raindrop::INVALID_SCENE_PTR;
	EXPECT_NO_THROW(scene = app.createScene(500));
	EXPECT_NE(scene, Raindrop::INVALID_SCENE_PTR);

	EXPECT_NO_THROW(app.destroySceen(scene));
	scene = Raindrop::INVALID_SCENE_PTR;
}