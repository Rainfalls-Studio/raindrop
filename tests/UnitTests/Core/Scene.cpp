#include <typeinfo>
#include <stdexcept>

#include <gtest/gtest.h>
#include <Raindrop.hpp>

using namespace Raindrop::Core::Scene;

TEST(Signature, init){
	Signature sig;
}

TEST(Signature, validSet){
	Signature sig;
	EXPECT_NO_THROW(sig.set(35, true));
}

TEST(Signature, invalidSet){
	Signature sig;
	EXPECT_THROW(sig.set(87, true), std::out_of_range);
}

TEST(Signature, validGet){
	Signature sig;
	EXPECT_NO_THROW(sig.set(36, true));
	EXPECT_TRUE(sig.get(36));
	EXPECT_FALSE(sig.get(37));
}

TEST(Signature, invalidGet){
	Signature sig;
	EXPECT_THROW(sig.get(97), std::out_of_range);
}

TEST(signatureManager, init){
	SignatureManager manager(RAINDROP_default_allocator, 1000);
	EXPECT_EQ(manager.capacity(), 1000);
}

TEST(signatureManager, validSet){
	SignatureManager manager(RAINDROP_default_allocator, 1000);
	manager.setBit(100, 32, true);
	EXPECT_TRUE(manager.getBit(100, 32));
}

TEST(signatureManager, outOfRangeSetID){
	SignatureManager manager(RAINDROP_default_allocator, 1000);
	EXPECT_THROW(manager.setBit(1050, 32, true), std::out_of_range);
}

TEST(signatureManager, outOfRangeSetBit){
	SignatureManager manager(RAINDROP_default_allocator, 1000);
	EXPECT_THROW(manager.setBit(500, 89, true), std::out_of_range);
}

TEST(entityManager, init){
	EntityManager manager(RAINDROP_default_allocator);
}

TEST(entityManager, createEntity){
	EntityManager manager(RAINDROP_default_allocator);
	EXPECT_NO_THROW(manager.create());
}

TEST(entityManager, destroyEntity){
	EntityManager manager(RAINDROP_default_allocator);
	auto entity = manager.create();
	EXPECT_NO_THROW(manager.destroy(entity));
}

TEST(entityManger, invalidDestroy){
	EntityManager manager(RAINDROP_default_allocator);
	EXPECT_THROW(manager.destroy(50), std::invalid_argument);
}

TEST(entityManger, outOfRangeDestroy){
	EntityManager manager(RAINDROP_default_allocator);
	EXPECT_THROW(manager.destroy(5000), std::out_of_range);
}

TEST(entityManager, creationOverflow){
	EntityManager manager(RAINDROP_default_allocator, 1);
	auto entity = manager.create();
	EXPECT_THROW(manager.create(), std::overflow_error);
}

struct Component1{
	uint8_t members[10];
};

TEST(componentManager, init){
	ComponentManager manager(RAINDROP_default_allocator, sizeof(Component1), alignof(Component1));
}

TEST(componentManager, validGet){
	ComponentManager manager(RAINDROP_default_allocator, sizeof(Component1), alignof(Component1));
	EXPECT_NE(manager.get(100), nullptr);
}

TEST(ComponentManager, outOfRangeGet){
	ComponentManager manager(RAINDROP_default_allocator, sizeof(Component1), alignof(Component1));
	EXPECT_THROW(manager.get(5000), std::out_of_range);
}

TEST(componentManager, validSet){
	ComponentManager manager(RAINDROP_default_allocator, sizeof(Component1), alignof(Component1));
	Component1 comp{};
	EXPECT_NO_THROW(manager.set(500, &comp));
}

TEST(ComponentManager, invalidSet){
	ComponentManager manager(RAINDROP_default_allocator, sizeof(Component1), alignof(Component1));
	EXPECT_THROW(manager.set(500, nullptr), std::invalid_argument);
}

TEST(ComponentManager, outOfRangeSet){
	ComponentManager manager(RAINDROP_default_allocator, sizeof(Component1), alignof(Component1));
	Component1 comp{};
	EXPECT_THROW(manager.set(5000, &comp), std::out_of_range);
}

class SystemTest : public System{
	public:
		SystemTest(Raindrop::Core::Memory::Allocator& allocator) : System(allocator){}
		~SystemTest(){}

		auto& content() const{return this->_entities;}
};

TEST(systemManager, init){
	SystemManager manager(RAINDROP_default_allocator);
}

TEST(systemManager, pushValidSystem){
	SystemManager manager(RAINDROP_default_allocator);
	SystemTest system(RAINDROP_default_allocator);
	Signature sig;
	EXPECT_NO_THROW(manager.pushSystem(&system, sig));
}

TEST(systemManager, pushInvalidSystem){
	SystemManager manager(RAINDROP_default_allocator);
	Signature sig;
	EXPECT_THROW(manager.pushSystem(nullptr, sig), std::invalid_argument);
}

TEST(systemManager, popValidSystem){
	SystemManager manager(RAINDROP_default_allocator);
	SystemTest system(RAINDROP_default_allocator);
	Signature sig;
	EXPECT_NO_THROW(manager.pushSystem(&system, sig));
	EXPECT_NO_THROW(manager.popSystem(&system));
}

TEST(systemManager, popNonExistantSystem){
	SystemManager manager(RAINDROP_default_allocator);
	SystemTest system(RAINDROP_default_allocator);
	EXPECT_THROW(manager.popSystem(&system), std::invalid_argument);
}

TEST(systemManager, popInvalidSystem){
	SystemManager manager(RAINDROP_default_allocator);
	EXPECT_THROW(manager.popSystem(nullptr), std::invalid_argument);
}

TEST(systemManager, entityUpdate){
	SystemManager manager(RAINDROP_default_allocator);
	Raindrop::ID32 entity = 0;
	SystemTest system(RAINDROP_default_allocator);

	Signature sig;
	sig.set(2, true);

	EXPECT_NO_THROW(manager.pushSystem(&system, sig));
	EXPECT_NO_THROW(manager.entitySignatureUpdate(entity, Signature{}, sig));
}

TEST(systemManager, entityRemove){
	SystemManager manager(RAINDROP_default_allocator);
	Raindrop::ID32 entity = 0;
	SystemTest system(RAINDROP_default_allocator);

	Signature sig;
	sig.set(2, true);

	EXPECT_NO_THROW(manager.pushSystem(&system, sig));
	EXPECT_NO_THROW(manager.entitySignatureUpdate(entity, Signature{}, sig));
	EXPECT_NO_THROW(manager.entityRemoved(entity, sig));
}

TEST(systemSignature, entityUpdateSignature2){
	SystemManager manager(RAINDROP_default_allocator);
	Raindrop::ID32 entity = 0;
	SystemTest system(RAINDROP_default_allocator);

	Signature sig;
	sig.set(2, true);

	Signature sig2;
	sig.set(3, true);

	EXPECT_NO_THROW(manager.pushSystem(&system, sig));
	EXPECT_NO_THROW(manager.entitySignatureUpdate(entity, Signature{}, sig));
	EXPECT_NO_THROW(manager.entitySignatureUpdate(entity, sig, sig2));

	EXPECT_TRUE(system.content().empty());
}