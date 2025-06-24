#pragma once

#include "types.hpp"
#include "pch.pch"
#include <Raindrop/Object.hpp>

namespace Raindrop::Graphics{
	class Framebuffer : public Object{
		public:
			static std::shared_ptr<Framebuffer> create(Raindrop::Context& context);

			Framebuffer() noexcept;
			~Framebuffer();

			Framebuffer& prepare(Context& context);
			void initialize();
			void release();

			const VkFramebuffer& get() const noexcept;

			Framebuffer& setFlags(const VkFramebufferCreateFlags& flags);
			Framebuffer& setWidth(const std::uint32_t& width);
			Framebuffer& setHeight(const std::uint32_t& height);
			Framebuffer& setLayers(const std::uint32_t& layers);
			Framebuffer& setSize(const std::uint32_t& width, const std::uint32_t& height = 1, const std::uint32_t& layers = 1);
			Framebuffer& setRenderPass(const std::shared_ptr<RenderPass>& renderPass);
			Framebuffer& setAttachments(const std::list<std::shared_ptr<ImageView>>& attachments);

		private:
			struct BuildInfo{
				VkFramebufferCreateFlags flags;
				std::uint32_t width;
				std::uint32_t height;
				std::uint32_t layers;
				std::shared_ptr<RenderPass> renderPass;
				std::list<std::shared_ptr<ImageView>> attachments;
			};

			Context* _context;
			VkFramebuffer _framebuffer;
			std::shared_ptr<RenderPass> _renderPass;
			std::list<std::shared_ptr<ImageView>> _attachments;

			std::unique_ptr<BuildInfo> _info;
			BuildInfo& getInfo();
	};
}