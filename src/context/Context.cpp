#include "render/Context.hpp"

namespace rnd::render{
	Context* Context::create(API api){
		switch (api){
			case API::Vulkan: RND_ERR("render context creation", "the vulkan context is not available for now"); return nullptr;
			case API::Opengl: RND_ERR("render context creation", "the opengl context is not available for now"); return nullptr;
		}
		RND_ERR("render context create", "invalid render API");
		return nullptr;
	};
}