#ifndef __RAINDROP_RENDERER_BASE_FRAMEBUFFER_CONTEXT_HPP__
#define __RAINDROP_RENDERER_BASE_FRAMEBUFFER_CONTEXT_HPP__

#include "common.hpp"
#include "RenderPass.hpp"
#include "Framebuffer.hpp"
#include "Sampler.hpp"
#include "DescriptorSet.hpp"
#include "FullscreenQuad.hpp"

namespace Raindrop::Renderer::BaseFramebuffer{
	struct Context{
		using Renderer = ::Raindrop::Renderer::Context;
		Context(Renderer& renderer);

		Renderer& renderer;

		RenderPass renderPass;
		DescriptorSet set;
		Framebuffer framebuffer;
		Sampler sampler;
		FullscreenQuad fullscreenQuad;
	};
}

#endif