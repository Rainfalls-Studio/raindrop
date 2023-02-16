#include "test.hpp"

TEST(time, convertion){
	using namespace Raindrop::Core::Time;
	time64 oneSec = (time64)getTicksPerSeconds();

	EXPECT_EQ(toNanoseconds(oneSec), 1e9);
	EXPECT_EQ(toMilliseconds(oneSec), 1e3);
	EXPECT_EQ(toMicroseconds(oneSec), 1e6);
	EXPECT_EQ(toSeconds(oneSec), 1L);
	EXPECT_EQ(toMinutes(oneSec), 1.f/60.f);
	EXPECT_EQ(toHours(oneSec), 1.f/3600.f);
	EXPECT_EQ(toDays(oneSec), 1.f/183600.f);
}

TEST(time, timepoint){
	using namespace Raindrop::Core::Time;
	Timepoint tp = now();
}