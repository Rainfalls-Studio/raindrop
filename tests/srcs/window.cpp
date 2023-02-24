#include "test.hpp"

using namespace Raindrop;
using namespace Core;

TEST(window, init){
	Window::Window window("modules/window.dll");
	EXPECT_TRUE(window.loaded());

	window.setPosition({100, 100});
	EXPECT_EQ(window.getPosition(), Maths::vec2<uint32>(100, 100));
}