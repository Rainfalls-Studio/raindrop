#include "integrationTest.hpp"

Raindrop::Application ApplicationTest::application{"Test Application", RAINDROP_CreateVersion(0, 1, 0)};

TEST_F(ApplicationTest, initialization){
	EXPECT_EQ(strcmp(application.name(), "Test Application"), 0);
}