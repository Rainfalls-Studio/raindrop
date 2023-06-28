#include <Raindrop/Graphics/GraphicsContext.hpp>
#include <Raindrop/Graphics/Instance.hpp>
#include <Raindrop/Graphics/Device.hpp>
#include <Raindrop/Graphics/Window.hpp>

namespace Raindrop::Graphics{
	GraphicsContext::GraphicsContext(Core::EngineContext& context, Core::Scene::Scene& scene) :
		scene{scene},
		context{context},
		window{*this},
		instance{*this},
		device{*this},
		swapchain{*this},
		graphicsCommandPool{*this},
		transfertCommandPool{*this}{
	}

	GraphicsContext::~GraphicsContext(){
	}
}