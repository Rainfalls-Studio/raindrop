#include "integrationTest.hpp"

class ModuleTest : public ApplicationTest{
	public:
		ModuleTest() = default;
		virtual ~ModuleTest() = default;

		static void SetUpTestCase(){
			module = application.loadOrGetModule(data / "module.dll");
		}

		static Raindrop::Module module;
};

Raindrop::Module ModuleTest::module;

TEST_F(ModuleTest, initialization){
	EXPECT_TRUE(application.isModuleLoaded(data / "module.dll"));
}

TEST_F(ModuleTest, get_valid_function){
	auto fnc = module.getFnc<int>("testIntFunction");
	EXPECT_TRUE(fnc);
}

TEST_F(ModuleTest, get_invalid_function){
	EXPECT_ANY_THROW(module.getFnc<int>("invalidFunction"));
}

TEST_F(ModuleTest, load_plugin){
	auto plugin = application.createPlugin("TestPlugin");
	EXPECT_TRUE(static_cast<bool>(plugin));

	EXPECT_EQ(strcmp(plugin->name(), "Test Plugin"), 0);
}