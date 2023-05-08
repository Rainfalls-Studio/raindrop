#include <test.hpp>

using namespace Raindrop::Core::IO;

static std::filesystem::path module_path = std::filesystem::path(TEST_DATA_DIR) / "module.dll";

class PluginFixture : public ::testing::Test{
	public:
		PluginFixture() : Test(){
			_plugin = Plugin::createPlugin(_module, "testPlugin");
		}
	
		virtual void SetUp() override{}
		virtual void TearDown() override{}
		
		Module _module{module_path};
		std::shared_ptr<Plugin> _plugin;
};

TEST_F(PluginFixture, initialize){
	EXPECT_NE(_plugin.get(), nullptr);
}

TEST_F(PluginFixture, get_name){
	EXPECT_EQ(strcmp(_plugin->name(), "Test Plugin"), 0);
}

TEST_F(PluginFixture, get_version){
	EXPECT_EQ(_plugin->version(), Raindrop::Version(0, 1, 0));
}