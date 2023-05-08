#include <test.hpp>

using namespace Raindrop::Core::Scene;

class EntityManagerFixture : public ::testing::Test{
	public:
		EntityManagerFixture() : Test(){}
	
		virtual void SetUp() override{}
		virtual void TearDown() override{}

		EntityManager _manager{1024};
};

TEST_F(EntityManagerFixture, initialization){
	EXPECT_EQ(_manager.capacity(), 1024);
}

TEST_F(EntityManagerFixture, valid_create){
	EntityID id = INVALID_ENTITY;
	EXPECT_NO_THROW(id = _manager.create());
}

TEST_F(EntityManagerFixture, valid_destroy){
	EntityID id = _manager.create();
	EXPECT_NO_THROW(_manager.destroy(id));
}