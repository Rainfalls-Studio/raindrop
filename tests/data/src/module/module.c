#if defined(_WIN32) || defined(__CYGWIN__)
	#define API __declspec(dllexport)
#else
	#define API __attribute__((visibility("default")))
#endif

void API testVoidFunction(){
}

int API testIntFunction(){
	return 42;
}

float API testFloatParamaterFunction(float a){
	return a;
}