#include <UnitTest++/UnitTest++.h>
#include <Raindrop.hpp>

using namespace Raindrop::Core::Time;

SUITE(time){
	TEST(sleep){
		utime64 before = getTicks();
		sleep(1000);
		utime64 after = getTicks();
		CHECK_CLOSE(after - before, getTicksPerSeconds(), 1000);
	}

	TEST(to_seconds){
		utime64 oneSec = getTicksPerSeconds();
		CHECK_CLOSE(toSeconds(oneSec), 1.f, FLT_MIN);
	}

	TEST(to_milliseconds){
		utime64 oneSec = getTicksPerSeconds();
		CHECK_CLOSE(toMilliseconds(oneSec), 1e3, FLT_MIN);
	}

	TEST(to_microseconds){
		utime64 oneSec = getTicksPerSeconds();
		CHECK_CLOSE(toMilliseconds(oneSec), 1e6, FLT_MIN);
	}

	TEST(to_nanoseconds){
		utime64 oneSec = getTicksPerSeconds();
		CHECK_CLOSE(toMilliseconds(oneSec), 1e9, FLT_MIN);
	}

	TEST(to_minutes){
		utime64 oneSec = getTicksPerSeconds();
		CHECK_CLOSE(toMinutes(oneSec), 1.f/60.f, FLT_MIN);
	}

	TEST(to_hours){
		utime64 oneSec = getTicksPerSeconds();
		CHECK_CLOSE(toHours(oneSec), 1.f/3600.f, FLT_MIN);
	}

	TEST(to_days){
		utime64 oneSec = getTicksPerSeconds();
		CHECK_CLOSE(toDays(oneSec), 1.f/86400.f, FLT_MIN);
	}
}

SUITE(duration){
	TEST(constructor_time_64){
		Duration duration(5000);
		CHECK_EQUAL(duration.duration(), 5000);
	}

	TEST(constructor_const_ref_copy){
		Duration base(5000);
		Duration duration(base);
		CHECK_EQUAL(duration.duration(), 5000);
	}

	TEST(operator_multiply){
		Duration duration(5);
		Duration x = duration * 5;
		CHECK_EQUAL(x.duration(), 25);
	}

	TEST(operator_devide){
		Duration duration(5);
		Duration x = duration / 5;
		CHECK_EQUAL(x.duration(), 1);
	}

	TEST(operator_add){
		Duration duration(5);
		Duration x = duration + 5;
		CHECK_EQUAL(x.duration(), 10);
	}

	TEST(operator_minus){
		Duration duration(5);
		Duration x = duration - 5;
		CHECK_EQUAL(x.duration(), 0);
	}
}
