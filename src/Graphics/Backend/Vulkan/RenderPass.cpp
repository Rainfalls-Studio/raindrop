#include <Raindrop/Graphics/Backend/Vulkan/RenderPass.hpp>
#include <Raindrop/Graphics/Backend/Vulkan/Context.hpp>
#include <vulkan/vulkan_core.h>

namespace Raindrop::Graphics::Backend::Vulkan{
	RenderPass::RenderPass(Context& context, VkRenderPass renderPass) noexcept : 
		_context{&context},
		_renderPass{renderPass}
	{}

	RenderPass::~RenderPass(){
        auto& device = _context->device;
		
		if (_renderPass != VK_NULL_HANDLE){
			_context->logger->info("Destroying render pass ...");

			vkDestroyRenderPass(device, _renderPass, nullptr);
			_renderPass = VK_NULL_HANDLE;
		}
	}

	RenderPass::RenderPass(RenderPass&& other) : 
		_context{nullptr},
		_renderPass{VK_NULL_HANDLE}{
		swap(*this, other);
	}

	RenderPass& RenderPass::operator=(RenderPass&& other){
		swap(*this, other);
		return *this;
	}

	void swap(RenderPass& A, RenderPass& B){
		std::swap(A._context, B._context);
		std::swap(A._renderPass, B._renderPass);
	}

	const VkRenderPass& RenderPass::get() const noexcept{
		return _renderPass;
    }
}