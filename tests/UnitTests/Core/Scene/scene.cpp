#include <Raindrop/Core/Scene/Scene.hpp>
#include <gtest/gtest.h>

class SceneTest : public ::testing::Test{
	public:
		virtual ~SceneTest() override = default;

		Raindrop::Core::Scene::Scene scene;
};

TEST_F(SceneTest, initialization){
	
}