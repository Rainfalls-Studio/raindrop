#include <typeinfo>
#include <stdexcept>

#include <gtest/gtest.h>
#include <Raindrop.hpp>


using namespace Raindrop::Core::Thread;

// dummy function who just return it argument
void* foo(void* arg){
	return static_cast<int*>(arg);
}

TEST(thread, self){
	Thread thread = self();
	EXPECT_EQ(thread.id(), Thread::self().id());
}

TEST(thread, createValidThread){
	int arg = 12345;
	Thread thread(&foo, &arg);
	EXPECT_EQ(arg, 12345);
}

TEST(thread, createInvalidThread){
	int arg = 12345;
	EXPECT_THROW(Thread(nullptr, &arg), std::invalid_argument);
}

TEST(thread, detachTHread){
	int arg = 12345;
	Thread thread(&foo, &arg);
	thread.detach();
}

TEST(thread, jointThread){
	int arg = 12345;
	Thread thread(&foo, &arg);

	int result = 12345;
	thread.join(&result);
	EXPECT_EQ(result, arg);
}

TEST(thread, kill){
	int arg = 12345;
	Thread thread(&foo, &arg);
	thread.kill(Thread::SIGNAL_TERMINATE);
}

TEST(thread, cancel){
	int arg = 12345;
	Thread thread(&foo, &arg);
	thread.cancel();
}