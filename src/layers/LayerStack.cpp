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
		nextID=0;
	}

	void LayerStack::update(const FrameData& data){
		PROFILE_FUNCTION();

		for (auto &l : layers){
			l.layer->update(data);
		}

	}
}