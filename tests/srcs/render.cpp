#include "test.hpp"

using namespace Raindrop;
namespace gfx = Raindrop::Graphics;
namespace core = Raindrop::Core;
namespace IO = Raindrop::Core::IO;
namespace mem = Raindrop::Core::Memory;
namespace maths = Raindrop::Core::Maths;

// TEST(opengl, init){
// 	void* ptr = malloc(MEMORY_MO);
// 	IO::Module module("modules/openglRenderModule.dll");
	
// 	{
// 		mem::FreelistAllocator allocator(MEMORY_MO * 255, ptr);
// 		gfx::Window* window = gfx::Window::create(allocator, module);
// 		EXPECT_TRUE(window->loaded());

// 		window->setTitle("new title");
// 		EXPECT_EQ(strcmp(window->getTitle(), "new title"), 0);

// 		window->setPosition({0, 0});
// 		EXPECT_EQ(window->getPosition(), maths::vec2<uint32>(0, 0));

// 		window->setSize({150, 150});
// 		EXPECT_EQ(window->getSize(), maths::vec2<uint32>(150, 150));

// 		mem::deallocateDelete(allocator, *window);
// 	}

// 	free(ptr);
// }

// TEST(opengl, renderer){
// 	void* ptr = malloc(MEMORY_MO);
// 	IO::Module module("modules/openglRenderModule.dll");
	
// 	{
// 		mem::FreelistAllocator allocator(MEMORY_MO, ptr);
// 		gfx::Renderer* renderer = gfx::Renderer::create(allocator, module);

// 		EXPECT_TRUE(renderer->loaded());
		
// 		mem::deallocateDelete(allocator, *renderer);
// 	}

// 	free(ptr);
// }

// TEST(opengl, context){
// 	void* ptr = malloc(MEMORY_MO);
// 	IO::Module module("modules/openglRenderModule.dll");
	
// 	{
// 		mem::FreelistAllocator allocator(MEMORY_MO, ptr);
// 		gfx::Renderer* renderer = gfx::Renderer::create(allocator, module);
// 		gfx::Window* window = gfx::Window::create(allocator, module);
// 		gfx::Context* context = gfx::Context::create(allocator, *renderer, *window);

// 		EXPECT_NE(context, nullptr);
		
// 		mem::deallocateDelete(allocator, *context);
// 		mem::deallocateDelete(allocator, *window);
// 		mem::deallocateDelete(allocator, *renderer);
// 	}

// 	free(ptr);
// }

TEST(vulkan, init){
	void* ptr = malloc(MEMORY_MO);
	IO::Module module("modules/vulkanRenderModule.dll");
	
	{
		mem::FreelistAllocator allocator(MEMORY_MO, ptr);
		gfx::Window* window = gfx::Window::create(allocator, module);
		EXPECT_TRUE(window->loaded());

		window->setTitle("new title");
		EXPECT_EQ(strcmp(window->getTitle(), "new title"), 0);

		window->setPosition({0, 0});
		EXPECT_EQ(window->getPosition(), maths::vec2<uint32>(0, 0));

		window->setSize({150, 150});
		EXPECT_EQ(window->getSize(), maths::vec2<uint32>(150, 150));

		mem::deallocateDelete(allocator, *window);
	}

	free(ptr);
}

TEST(vulkan, renderer){
	void* ptr = malloc(MEMORY_MO);
	IO::Module module("modules/vulkanRenderModule.dll");
	
	{
		mem::FreelistAllocator allocator(MEMORY_MO, ptr);
		gfx::Renderer* renderer = gfx::Renderer::create(allocator, module);

		EXPECT_TRUE(renderer->loaded());
		
		mem::deallocateDelete(allocator, *renderer);
	}

	free(ptr);
}

TEST(vulkan, context){
	void* ptr = malloc(MEMORY_MO);
	IO::Module module("modules/vulkanRenderModule.dll");
	
	{
		mem::FreelistAllocator allocator(MEMORY_MO, ptr);
		gfx::Renderer* renderer = gfx::Renderer::create(allocator, module);
		gfx::Window* window = gfx::Window::create(allocator, module);
		gfx::Context* context = gfx::Context::create(allocator, *renderer, *window);

		EXPECT_NE(context, nullptr);
		context->enableVSync(false);

		if (context->begin()){
			context->end();
		}
		
		mem::deallocateDelete(allocator, *context);
		mem::deallocateDelete(allocator, *window);
		mem::deallocateDelete(allocator, *renderer);
	}

	free(ptr);
}