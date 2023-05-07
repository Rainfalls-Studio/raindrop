#include <test.hpp>

using namespace Raindrop::Core::IO;

using TestVoidFunction_t = void(*)(void);
using TestIntFunction_t = int(*)(void);
using TestFloatParamaterFunction_t = float(*)(float);

static std::filesystem::path module_path = std::filesystem::path(TEST_DATA_DIR) / "module.dll";

class ModuleFixture : public ::testing::Test{
	public:
		ModuleFixture() : Test(){}
	
		virtual void SetUp() override{}
		virtual void TearDown() override{}

		Module _module{module_path};
};

TEST_F(ModuleFixture, intialization){
	EXPECT_TRUE(_module.isOpen());
	EXPECT_EQ(_module.filepath(), module_path);
}

TEST_F(ModuleFixture, valid_get_proc){
	TestVoidFunction_t fnc = nullptr;
	EXPECT_NO_THROW(fnc = (TestVoidFunction_t)_module.getFnc("testVoidFunction"));
	
	EXPECT_NE(fnc, nullptr);
	EXPECT_NO_FATAL_FAILURE(fnc());
}

TEST_F(ModuleFixture, invalid_get_proc){
	TestVoidFunction_t fnc = nullptr;
	EXPECT_ANY_THROW(fnc = (TestVoidFunction_t)_module.getFnc("nonexistantFonction"));
}