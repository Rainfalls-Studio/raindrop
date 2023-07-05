#ifndef __RAINDROP_GRAPHICS_WORLD_FRAMEBUFFER_HPP__
#define __RAINDROP_GRAPHICS_WORLD_FRAMEBUFFER_HPP__

#include <Raindrop/Graphics/common.hpp>

namespace Raindrop::Graphics{
	class WorldFramebuffer : public Target{
		public:
			struct Attachment{
				VkDeviceMemory memory = VK_NULL_HANDLE;
				VkImage image = VK_NULL_HANDLE;
				VkImageView imageView = VK_NULL_HANDLE;
			};

			WorldFramebuffer(GraphicsContext& context, uint32_t width, uint32_t height);
			~WorldFramebuffer();

			virtual VkRenderPass renderPass() const override;
			virtual uint32_t attachmentCount() const override;

			void beginRenderPass(VkCommandBuffer commandBuffer);
			void endRenderPass(VkCommandBuffer commandBuffer);

			Attachment& getAttachment(uint32_t id);
			std::vector<Attachment>& getAttachments();

		private:
			GraphicsContext& _context;

			VkFramebuffer _framebuffer;
			VkRenderPass _renderPass;

			std::vector<Attachment> _attachments;
			std::weak_ptr<GraphicsPipeline> _pipeline;

			uint32_t _width;
			uint32_t _height;

			void createRenderPass();
			void createAttachments();
			void createFramebuffer();
			void checkDefaultFormats();
	};
}

#endif