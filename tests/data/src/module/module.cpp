#include <Raindrop/Raindrop.hpp>

class Plugin : public ::Raindrop::Core::IO::Plugin{
	public:
		Plugin() = default;
		virtual ~Plugin() override = default;

		virtual ::Raindrop::Version version() const override{
			return ::Raindrop::Version(0, 1, 0);
		}

		virtual const char* name() const override{
			return "Test Plugin";
		}
};

RAINDROP_MODULE std::shared_ptr<Plugin> testPlugin(){
	return std::make_shared<Plugin>();
}

void aa(){
	__RAINDROP_log_getContext();
	__RAINDROP_log_setContext(nullptr);
	__RAINDROP_profile_getContext();
	__RAINDROP_profile_setContext(nullptr);
}

RAINDROP_MODULE void testVoidFunction(){
}

RAINDROP_MODULE int testIntFunction(){
	return 42;
}

RAINDROP_MODULE float testFloatParamaterFunction(float a){
	return a;
}