#include "raindrop.hpp"

int main(int argc, char** argv){

	RND_LOG_INIT("out.log");
	PROFILE_BEGIN_SESSION("test", "profile.json");
	PROFILE_RECORD();
	
	rnd::memory::allocators::ChunkAllocator<int> allocator;

	allocator.init(500);
	RND_LOG("initialization", "success");

	for (int i=0; i<500; i++){
		allocator.allocate();
	}
	RND_LOG("filling", "success");

	allocator.clear();
	RND_LOG("clearing", "success");

	for (int i=0; i<1500; i++){
		allocator.allocate();
	}
	RND_LOG("dynamic allocation", "success");

	for (int i=0; i<500; i++){
		allocator.deallocate(allocator.allocate());
	}
	RND_LOG("deallocator", "success");

	allocator.shutdown();
	RND_LOG("shutdown", "success");

	PROFILE_END_SESSION();
	RND_LOG_SHUTDOWN();

	return EXIT_SUCCESS;
}