#include "test.hpp"

TEST(profiler, begin_end){
	RAINDROP_profile_begin("begin_end", "begin.csv");
	// RAINDROP_profile(false);
	{
		RAINDROP_profile_scope("scope");
	}
	RAINDROP_profile_end();
}