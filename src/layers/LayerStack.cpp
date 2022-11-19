#include "layers/LayerStack.hpp"

namespace rnd::layers{
	void LayerStack::init(){
		PROFILE_FUNCTION();

	}

	void LayerStack::shutdown(){
		PROFILE_FUNCTION();
		clear();

	}

	void LayerStack::clear(){
		PROFILE_FUNCTION();

		for (auto &l : layers){
			l.layer->~Layer();
			memory::free(l.layer);
		}
		layers.clear();
	}
}