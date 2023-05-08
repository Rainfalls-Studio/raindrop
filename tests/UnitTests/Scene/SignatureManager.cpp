#include <test.hpp>

using namespace Raindrop::Core::Scene;

class SignatureFixture : public ::testing::Test{
	public:
		SignatureFixture() : Test(){}
	
		virtual void SetUp() override{}
		virtual void TearDown() override{}

		Signature _signature;
};

TEST_F(SignatureFixture, initialization){
	EXPECT_FALSE(static_cast<bool>(_signature));
}

TEST_F(SignatureFixture, valid_set){
	EXPECT_NO_THROW(_signature.set(5, true));
}

TEST_F(SignatureFixture, invalid_set){
	EXPECT_ANY_THROW(_signature.set(582442, true));
}

TEST_F(SignatureFixture, valid_get){
	_signature.set(5, true);
	bool state = false;
	EXPECT_NO_THROW(state = _signature.get(5));
	EXPECT_TRUE(state);
}

TEST_F(SignatureFixture, invalid_get){
	EXPECT_ANY_THROW(_signature.get(4815165));
}

class SignatureManagerFixture : public ::testing::Test{
	public:
		SignatureManagerFixture() : Test(){}
	
		virtual void SetUp() override{}
		virtual void TearDown() override{}

		SignatureManager _manager{1024};
};

TEST_F(SignatureManagerFixture, initialization){
	EXPECT_EQ(_manager.capacity(), 1024);
}

TEST_F(SignatureManagerFixture, valid_get){
	Signature sig;
	EXPECT_NO_THROW(sig = _manager.get(54));
}

TEST_F(SignatureManagerFixture, invalid_get){
	EXPECT_ANY_THROW(_manager.get(72587654));
}