#include <Raindrop/Raindrop.hpp>

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