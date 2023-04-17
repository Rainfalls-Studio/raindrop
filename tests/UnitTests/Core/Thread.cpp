// #include <typeinfo>
// #include <stdexcept>

// #include <gtest/gtest.h>
// #include <Raindrop.hpp>


// using namespace Raindrop::Core::Thread;

// // dummy function who just return it argument
// void* foo(void* arg){
// 	return static_cast<int*>(arg);
// }

// TEST(thread, self){
// 	Thread thread = self();
// 	EXPECT_EQ(thread.id(), Thread::self().id());
// }

// TEST(thread, createValidThread){
// 	int arg = 12345;
// 	Thread thread(&foo, &arg);
// 	EXPECT_EQ(arg, 12345);
// }

// TEST(thread, createInvalidThread){
// 	int arg = 12345;
// 	EXPECT_THROW(Thread(nullptr, &arg), std::invalid_argument);
// }

// TEST(thread, detachTHread){
// 	int arg = 12345;
// 	Thread thread(&foo, &arg);
// 	thread.detach();
// }

// TEST(thread, jointThread){
// 	int arg = 12345;
// 	Thread thread(&foo, &arg);

// 	int result = 12345;
// 	thread.join(&result);
// 	EXPECT_EQ(result, arg);
// }

// TEST(thread, kill){
// 	int arg = 12345;
// 	Thread thread(&foo, &arg);
// 	thread.kill(Thread::SIGNAL_TERMINATE);
// }

// TEST(thread, cancel){
// 	int arg = 12345;
// 	Thread thread(&foo, &arg);
// 	thread.cancel();
// }

// // Mutex

// TEST(mutex, initialization){
// 	Mutex mutex;
// }

// TEST(mutex, lock){
// 	Mutex mutex;
// 	mutex.lock();
// }

// TEST(mutex, invalidUnlock){
// 	Mutex mutex;
// 	mutex.unlock();
// }

// TEST(mutex, validUnlock){
// 	Mutex mutex;
// 	mutex.lock();
// 	mutex.unlock();
// }

// TEST(mutex, tryLock){
// 	Mutex mutex;
// 	EXPECT_TRUE(mutex.tryLock());
// }

// // conditional variables

// void* sendSignalFnc(void* ptr){
// 	ConditionalVariable& cond = *reinterpret_cast<ConditionalVariable*>(ptr);

// 	// wait to ensure that the main thread hit the condition
// 	Raindrop::Core::Time::sleep(1);
// 	cond.signal();
// 	return nullptr;
// }

// TEST(conditionalVariable, initialization){
// 	ConditionalVariable cond;
// }

// TEST(conditionalVariable, signal){
// 	ConditionalVariable cond;
// 	cond.signal();
// }

// TEST(conditionalVariable, broadcast){
// 	ConditionalVariable cond;
// 	cond.broadcast();
// }

// TEST(conditionalVariable, wait){
// 	ConditionalVariable cond;
// 	Mutex mutex;
// 	Thread thread(&sendSignalFnc, &cond);

// 	cond.wait(mutex);
// 	thread.join();
// }

// // barrier

// TEST(barrier, initialization){
// 	Barrier barrier(1);
// }

// TEST(barrier, wait){
// 	Barrier barrier(1);
// 	barrier.wait();
// }

// TEST(barrier, count){
// 	Barrier barrier(2);
// 	EXPECT_EQ(barrier.count(), 2);
// }