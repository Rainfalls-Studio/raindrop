#ifndef __RAINDROP_GRAPHICS_RENDERER_HPP__
#define __RAINDROP_GRAPHICS_RENDERER_HPP__

#include <Raindrop/Graphics/common.hpp>
#include <Raindrop/Graphics/factory/ShaderFactory.hpp>
#include <Raindrop/Graphics/factory/GraphicsPipelineFactory.hpp>
#include <Raindrop/Graphics/factory/ModelFactory.hpp>
#include <Raindrop/Graphics/GUI/Interpreter.hpp>
#include <Raindrop/Graphics/GraphicsContext.hpp>

namespace Raindrop::Graphics{
	class Renderer{
		friend class ImGUI;
		public:
			Renderer(Core::EngineContext& context, Core::Scene::Scene& scene);
			~Renderer();

			void update();
			void openGUI(const std::filesystem::path& path);

		private:
			std::unique_ptr<GraphicsContext> _context;
			std::unique_ptr<ImGUI> _gui;
			GUI::Interpreter _interpreter;

			std::vector<VkCommandBuffer> _graphicsCommandBuffers;

			std::shared_ptr<Factory::GraphicsPipelineFactory> _graphicsPipelineFactory;
			std::shared_ptr<Factory::ShaderFactory> _shaderFactory; 
			std::shared_ptr<Factory::ModelFactory> _modelFactory;

			std::unique_ptr<WorldFramebuffer> _worldFramebuffer;

			void registerFactories();
			void registerShaderFactory();
			void registerGraphicsPipelineFactory();
			void registerModelFactory();
			void eraseFactories();

			void createGraphicsCommandBuffers();
			void destroyGraphicsCommandBuffers();
			
			VkCommandBuffer getCurrentGraphicsCommandBuffer();
			
			VkCommandBuffer beginFrame();
			void endFrame();

			void renderGui();
			void renderScene(VkCommandBuffer commmandBuffer);
			void renderSwapchain(VkCommandBuffer commandBuffer);
	};

	struct PushConstant{
		glm::mat4 viewTransform;
		glm::mat4 localTransform;
	};
}

#endif