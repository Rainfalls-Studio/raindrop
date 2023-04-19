#include <Raindrop/Raindrop.hpp>
#include <gtest/gtest.h>

TEST(application, valid_initialization){
	EXPECT_NO_THROW(Raindrop::Application app("app", RAINDROP_CreateVersion(1, 0, 0)));
}

TEST(application, create_valid_scene){
	Raindrop::Application app("app", RAINDROP_CreateVersion(1, 0, 0));
	Raindrop::Scene scene = app.createScene(1000);

	EXPECT_EQ(scene.capacity(), 1000);
	EXPECT_NE(scene.getScenePtr(), nullptr);
}