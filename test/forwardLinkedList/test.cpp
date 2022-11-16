#include "raindrop.hpp"

int main(int argc, char** argv){

	RND_LOG_INIT("out.log");
	PROFILE_BEGIN_SESSION("test", "profile.json");
	PROFILE_RECORD();

	RND_LOG("default allocator", "");
	{
		rnd::memory::ForwardLinkedList<int, rnd::memory::allocators::Allocator, true> list;
		list.init();
		RND_LOG("initialization", "success");

		for (int i=0; i<500; i++){
			list.push(i);
		}
		RND_LOG("push", "success");

		for (int i=0; i<500; i++){
			list.pop();
		}
		RND_LOG("pop", "success");

		list.shutdown();
		RND_LOG("shutdown", "success");
	}

	{
		RND_LOG("chunk allocator", "");
		rnd::memory::allocators::ChunkAllocator<rnd::memory::ForwardLinkedListNode<int>> allocator;

		rnd::memory::ForwardLinkedList<int, rnd::memory::allocators::ChunkAllocator, true> list;
		list.init(&allocator);
		RND_LOG("initialization", "success");

		for (int i=0; i<500; i++){
			list.push(i);
		}
		RND_LOG("push", "success");

		for (int i=0; i<500; i++){
			list.pop();
		}
		RND_LOG("pop", "success");

		list.shutdown();
		RND_LOG("shutdown", "success");
	}

	RND_LOG("end", "end successfully");

	PROFILE_END_SESSION();
	RND_LOG_SHUTDOWN();

	return EXIT_SUCCESS;
}