#include <Raindrop/Graphics/Backend/Vulkan/RenderPass.hpp>
#include <Raindrop/Graphics/Backend/Vulkan/Context.hpp>
#include <vulkan/vulkan_core.h>

namespace Raindrop::Graphics::Backend::Vulkan{
	RenderPass::RenderPass(Context& context, const Description& renderPass) : 
		_context{context}
	{

	}

	RenderPass::~RenderPass(){
		
	}

	const VkRenderPass& RenderPass::get() const noexcept{
		return _renderPass;
    }

	void* RenderPass::getHandle() const noexcept{
		return static_cast<void*>(_renderPass);
	}

	API RenderPass::getAPI() const noexcept{
		return API::VULKAN;
	}
}