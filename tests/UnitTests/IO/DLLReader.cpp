#include <Raindrop/Raindrop.hpp>
#include <gtest/gtest.h>
#include <test.hpp>

using namespace Raindrop::Core::IO;

using TestVoidFunction_t = void(*)(void);
using TestIntFunction_t = int(*)(void);
using TestFloatParamaterFunction_t = float(*)(float);

std::filesystem::path module_path = std::filesystem::path(TEST_DATA_DIR) / "module.dll";

class DLLReaderFixture : public ::testing::Test{
	public:
		DLLReaderFixture() : Test(){}
	
		virtual void SetUp() override{}
		virtual void TearDown() override{}

		DLLReader _dll{module_path};
};

TEST_F(DLLReaderFixture, initialization){
	EXPECT_TRUE(_dll.isOpen());
	EXPECT_EQ(_dll.filepath(), module_path);
}

TEST_F(DLLReaderFixture, valid_get_proc){
	TestVoidFunction_t fnc = nullptr;
	EXPECT_NO_THROW(fnc = (TestVoidFunction_t)_dll.getProc("testVoidFunction"));
	
	EXPECT_NE(fnc, nullptr);
	EXPECT_NO_FATAL_FAILURE(fnc());
}

TEST_F(DLLReaderFixture, invalid_get_proc){
	TestVoidFunction_t fnc = nullptr;
	EXPECT_ANY_THROW(fnc = (TestVoidFunction_t)_dll.getProc("nonexistantFonction"));
}