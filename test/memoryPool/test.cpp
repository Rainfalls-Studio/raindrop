#include "raindrop.hpp"
#include "core.hpp"

int main(int argc, char** argv){

	LOG_INIT("out.log");
	PROFILE_BEGIN_SESSION("test", "profile.json");
	PROFILE_RECORD();

	rnd::memory::Pool pool;
	pool.init<int>(50);
	
	int* value = pool.pop<int>();

	pool.shutdown();

	LOG(rnd::debug::Info, "end", "end successfully");
	PROFILE_END_SESSION();
	LOG_SHUTDOWN();

	return EXIT_SUCCESS;
}