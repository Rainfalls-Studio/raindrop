#include <gtest/gtest.h>
#include <Raindrop/Raindrop.hpp>

// using namespace Raindrop::Core::Time;

// // Time basics convertion functions

// TEST(time, to_seconds){
// 	utime64 oneSec = getTicksPerSeconds();
// 	EXPECT_FLOAT_EQ(toSeconds(oneSec), 1.f);
// }

// TEST(time, to_milliseconds){
// 	utime64 oneSec = getTicksPerSeconds();
// 	EXPECT_FLOAT_EQ(toMilliseconds(oneSec), 1e3);
// }

// TEST(time, to_microseconds){
// 	utime64 oneSec = getTicksPerSeconds();
// 	EXPECT_FLOAT_EQ(toMicroseconds(oneSec), 1e6);
// }

// TEST(time, to_nanoseconds){
// 	utime64 oneSec = getTicksPerSeconds();
// 	EXPECT_FLOAT_EQ(toNanoseconds(oneSec), 1e9);
// }

// TEST(time, to_minutes){
// 	utime64 oneSec = getTicksPerSeconds();
// 	EXPECT_FLOAT_EQ(toMinutes(oneSec), 1.f/60.f);
// }

// TEST(time, to_hours){
// 	utime64 oneSec = getTicksPerSeconds();
// 	EXPECT_FLOAT_EQ(toHours(oneSec), 1.f/3600.f);
// }

// TEST(time, to_days){
// 	utime64 oneSec = getTicksPerSeconds();
// 	EXPECT_FLOAT_EQ(toDays(oneSec), 1.f/86400.f);
// }