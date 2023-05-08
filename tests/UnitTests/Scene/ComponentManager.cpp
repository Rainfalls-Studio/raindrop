#include <test.hpp>

using namespace Raindrop::Core::Scene;

struct TestComponent{
	int a;
	float b;
	char c[255];
};

class ComponentManagerFixture : public ::testing::Test{
	public:
		ComponentManagerFixture() : Test(){}
	
		virtual void SetUp() override{}
		virtual void TearDown() override{}

		ComponentManager _manager{sizeof(TestComponent), alignof(TestComponent)};
};

TEST_F(ComponentManagerFixture, initialization){
	EXPECT_EQ(_manager.capacity(), 1024);
}

TEST_F(ComponentManagerFixture, valid_get){
	TestComponent* component = nullptr;
	EXPECT_NO_THROW(component = static_cast<TestComponent*>(_manager.get(500)));
	EXPECT_NE(component, nullptr);
}

TEST_F(ComponentManagerFixture, valid_set){
	std::unique_ptr<TestComponent> component = std::make_unique<TestComponent>();
	component->a = 152;
	component->b = 127.586f;
	strcpy(component->c, "hello world!");

	EXPECT_NO_THROW(_manager.set(250, component.get()));
}

TEST_F(ComponentManagerFixture, out_of_bound_get){
	EXPECT_ANY_THROW(_manager.get(78502));
}

TEST_F(ComponentManagerFixture, out_of_bound_set){
	std::unique_ptr<TestComponent> component = std::make_unique<TestComponent>();
	EXPECT_ANY_THROW(_manager.set(57545, component.get()));
}
 