#define NLOG
#include "test.hpp"

// =======================================================================================
// =======================================================================================
// =======================================================================================

using namespace Raindrop::Core::Debug::Log;

class LogModuleTest : public ::Raindrop::Core::Debug::Log::LogModule, public ::testing::Test{
	public:
		using Log = ::Raindrop::Core::Debug::Log::Log;
		virtual void callback(const Log &msg) override;
		
		void SetUp() override {
			push();
		}

		void TearDown() override {
			pop();
		}
};

void LogModuleTest::callback(const Log &msg){
	EXPECT_EQ(strcmp(severityToStr(msg.severity), "INFO"), 0);
	EXPECT_EQ(strcmp(causeToStr(msg.cause), "OTHER"), 0);
	EXPECT_EQ(strcmp(msg.file, "log.cpp"), 0);
	EXPECT_EQ(strcmp(msg.function, "virtual void LogModuleTest_severity_Test::TestBody()"), 0);
	EXPECT_EQ(strcmp(msg.condition, "none"), 0);
	EXPECT_EQ(strcmp(msg.message, "hello"), 0);
}

TEST_F(LogModuleTest, severity){
	RAINDROP_log(INFO, OTHER, "hello");
}
