#include <Raindrop/Graphics/Renderer.hpp>
#include <Raindrop/Graphics/Window.hpp>
#include <Raindrop/Graphics/PhysicalDevice.hpp>
#include <Raindrop/Graphics/Device.hpp>
#include <Raindrop/Graphics/Swapchain.hpp>
#include <Raindrop/Graphics/GraphicsPipeline.hpp>
#include <Raindrop/Graphics/ImGUI.hpp>
#include <Raindrop/Graphics/WorldFramebuffer.hpp>
#include <Raindrop/Graphics/Model.hpp>
#include <Raindrop/Core/Asset/AssetManager.hpp>
#include <Raindrop/Core/Scene/Entity.hpp>

#include <SDL2/SDL_vulkan.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <Raindrop/Core/Scene/Components/Camera.hpp>
#include <Raindrop/Core/Scene/Components/Hierarchy.hpp>
#include <Raindrop/Core/Scene/Components/Tag.hpp>
#include <Raindrop/Core/Scene/Components/Transform.hpp>
#include <Raindrop/Core/Scene/Components/Model.hpp>

namespace Raindrop::Graphics{
	Renderer::Renderer(Core::EngineContext& context, Core::Scene::Scene& scene) : _interpreter{context}{
		el::Logger* customLogger = el::Loggers::getLogger("Engine.Graphics");
		customLogger->configurations()->set(el::Level::Global, el::ConfigurationType::Format, "%datetime %level [%logger]: %msg");

		CLOG(INFO, "Engine.Graphics") << "Creating renderer ...";
		
		_context = std::make_unique<GraphicsContext>(context, scene);
		registerFactories();

		_gui = std::make_unique<ImGUI>(*_context);
		_worldFramebuffer = std::make_unique<WorldFramebuffer>(*_context, 1080, 720);

		createGraphicsCommandBuffers();

		CLOG(INFO, "Engine.Graphics") << "Created renderer with success !";
	}

	Renderer::~Renderer(){
		CLOG(INFO, "Engine.Graphics") << "Destroying renderer ...";

		_context->device.waitIdle();

		eraseFactories();
		destroyGraphicsCommandBuffers();

		_worldFramebuffer.reset();
		_gui.reset();
		_context.reset();
		
		CLOG(INFO, "Engine.Graphics") << "Destroyed renderer with success !";
	}

	void Renderer::openGUI(const std::filesystem::path& path){
		_interpreter.parse(path);
	}

	void drawEntity(Core::Scene::Entity entity, VkPipelineLayout layout, VkCommandBuffer commandBuffer, glm::mat4& viewTransform){

		auto& transform = entity.transform();
		glm::mat4 rotationMatrix = glm::eulerAngleXYZ(transform.rotation.x, transform.rotation.y, transform.rotation.z);
		glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), transform.translation);
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), transform.scale);

		PushConstant p;
		p.viewTransform = viewTransform;
		p.localTransform = translationMatrix * rotationMatrix * scaleMatrix;

		if (entity.hasComponent<Core::Scene::Components::Model>()){
			auto& model = entity.getComponent<Core::Scene::Components::Model>();

			if (auto m = model._model.lock()){
				vkCmdPushConstants(commandBuffer, layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstant), &p);
				m->draw(commandBuffer);
			}
		}

		for (auto c : entity){
			drawEntity(c, layout, commandBuffer, viewTransform);
		}
	}

	void Renderer::update(){
		auto& swapchain = _context->swapchain;

		_context->window.events(_gui.get());

		if (VkCommandBuffer commandBuffer = beginFrame()){

			_worldFramebuffer->beginRenderPass(commandBuffer);
			renderScene(commandBuffer);
			_worldFramebuffer->endRenderPass(commandBuffer);

			renderGui();

			swapchain.beginRenderPass(commandBuffer);
			renderSwapchain(commandBuffer);
			swapchain.endRenderPass(commandBuffer);

			endFrame();
		}
	}

	void Renderer::renderGui(){
		_gui->newFrame();

		auto& registry = _context->context.registry;
		auto& assetManager = _context->context.assetManager;
		auto& eventManager = _context->context.eventManager;
		auto& scene = _context->scene;

		_interpreter.update();
		registry["Edit.SelectedEntity"] = scene.UI(registry["Edit.SelectedEntity"].as<Core::Scene::EntityID>(Core::Scene::INVALID_ENTITY_ID));

		if (ImGui::Begin("component")){
			auto entity = registry["Edit.SelectedEntity"].as<Core::Scene::EntityID>();
			if (entity != Core::Scene::INVALID_ENTITY_ID) scene.componentsUI(entity);
		}
		ImGui::End();
	}

	void Renderer::renderSwapchain(VkCommandBuffer commandBuffer){
		_worldFramebuffer->render(commandBuffer);
		_gui->render(commandBuffer);
	}

	VkCommandBuffer Renderer::beginFrame(){
		auto& window = _context->window;
		auto& swapchain = _context->swapchain;

		VkResult result = swapchain.acquireNextImage();
		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			auto size = window.getSize();
			swapchain.setExtent(VkExtent2D{size.x, size.y});
			swapchain.rebuildSwapchain();
			return nullptr;
		}

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("failed to acquire swap chain image!");
		}

		auto commandBuffer = getCurrentGraphicsCommandBuffer();
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("failed to begin recording command buffer!");
		}
		return commandBuffer;
	}

	void Renderer::endFrame(){
		auto& window = _context->window;
		auto& swapchain = _context->swapchain;

		VkCommandBuffer commandBuffer = getCurrentGraphicsCommandBuffer();

		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
			throw std::runtime_error("failed to record command buffer");
		
		VkResult result = swapchain.submitCommandBuffer(&commandBuffer);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.resized()){
			auto size = window.getSize();
			window.resetResizedFlag();
			swapchain.setExtent(VkExtent2D{size.x, size.y});
			swapchain.rebuildSwapchain();
		} else if (result != VK_SUCCESS){
			throw std::runtime_error("failed to submit the command buffer");
		}
	}

	VkCommandBuffer Renderer::getCurrentGraphicsCommandBuffer(){
		return _graphicsCommandBuffers[_context->swapchain.currentFrame()];
	}

	void Renderer::registerFactories(){
		CLOG(INFO, "Engine.Graphics") << "registering renderer asset factories ...";
		registerShaderFactory();
		registerGraphicsPipelineFactory();
		registerModelFactory();
		CLOG(INFO, "Engine.Graphics") << "registred renderer asset factories with success !";
	}

	void Renderer::registerShaderFactory(){
		_shaderFactory = std::make_shared<Factory::ShaderFactory>(*_context);
		_context->context.assetManager.linkFactory(".spv", _shaderFactory);
	}

	void Renderer::registerGraphicsPipelineFactory(){
		_graphicsPipelineFactory = std::make_unique<Factory::GraphicsPipelineFactory>(*_context);
		_graphicsPipelineFactory->registerExtensions(_graphicsPipelineFactory);
	}

	void Renderer::registerModelFactory(){
		_modelFactory = std::make_unique<Factory::ModelFactory>(*_context);
		_modelFactory->registerExtensions(_modelFactory);
	}

	void Renderer::eraseFactories(){
		_shaderFactory.reset();
		_graphicsPipelineFactory.reset();
		_modelFactory.reset();
	}

	void Renderer::createGraphicsCommandBuffers(){
		_graphicsCommandBuffers.resize(_context->swapchain.frameCount());

		VkCommandBufferAllocateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		info.commandPool = _context->graphicsCommandPool.primary();
		info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		info.commandBufferCount = _graphicsCommandBuffers.size();

		if (vkAllocateCommandBuffers(_context->device.get(), &info, _graphicsCommandBuffers.data()) != VK_SUCCESS){
			CLOG(ERROR, "Engine.Graphics") << "Failed to allocate graphics command buffer";
			throw std::runtime_error("Failed to allocate graphics command buffers");
		}
	}

	void Renderer::destroyGraphicsCommandBuffers(){
		vkFreeCommandBuffers(_context->device.get(), _context->graphicsCommandPool.primary(), _graphicsCommandBuffers.size(), _graphicsCommandBuffers.data());
		_graphicsCommandBuffers.clear();
	}

	void Renderer::renderScene(VkCommandBuffer commandBuffer){
		auto& scene = _context->scene;

		glm::mat4 viewTransform;
		{
			auto& list = scene.componentEntities<Core::Scene::Components::Camera>();
			if (!list.empty()){
				auto entity = Core::Scene::Entity(list.front(), &scene);
				auto& component = entity.getComponent<Core::Scene::Components::Camera>();
				component.update(entity.getComponent<Core::Scene::Components::Transform>());
				viewTransform = component.viewProjection;
			} else {
				viewTransform = glm::scale(glm::mat4(1.f), glm::vec3(1/100.f));
			}
		}
		
		auto weak_pipeline = _context->context.registry["Pipeline"].as<std::weak_ptr<Raindrop::Core::Asset::Asset>>();
		if (auto pipeline = std::static_pointer_cast<GraphicsPipeline>(weak_pipeline.lock())){
			pipeline->bind(commandBuffer);
			
			VkRect2D scissor = VkRect2D{{0, 0}, {1080, 720}};
			VkViewport viewport = VkViewport{0, 0, 1080, 720, 0, 1};

			vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
			vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

			drawEntity(Core::Scene::Entity(scene.root(), &scene), pipeline->layout(), commandBuffer, viewTransform);
		}
	}
}