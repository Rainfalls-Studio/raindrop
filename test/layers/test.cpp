#include "raindrop.hpp"

class Layer1 : public rnd::layers::Layer{
	public:
		Layer1(int a) : Layer(){
			printf("layer1 constructor : %d\n", a);
		}

		~Layer1(){
			printf("destroy layer\n");
		}

	private:

};

int main(int argc, char** argv){

	RND_LOG_INIT("out.log");
	PROFILE_BEGIN_SESSION("test", "profile.json");
	PROFILE_RECORD();

	rnd::layers::LayerStack layerStack;
	layerStack.init();
	RND_LOG("initialize", "success");
	
	for (int i=0; i<50; i++){
		layerStack.pushLayer<Layer1>(i, i);
	}

	layerStack.shutdown();

	RND_LOG("shutdown", "success");

	PROFILE_END_SESSION();
	RND_LOG_SHUTDOWN();

	return EXIT_SUCCESS;
}