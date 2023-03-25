#include "test.hpp"

TEST(bitwise, shiftOffset){
	for (Raindrop::uint32 i=0; i<64; i++){
		Raindrop::uint64 value = 1ULL << i;
		EXPECT_EQ(i, Raindrop::Tools::getShiftOffset(value));
	}
}