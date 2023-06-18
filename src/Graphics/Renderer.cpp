#include <Raindrop/Graphics/Renderer.hpp>
#include <Raindrop/Graphics/Window.hpp>
#include <Raindrop/Graphics/builders/InstanceBuilder.hpp>
#include <Raindrop/Graphics/builders/DeviceBuilder.hpp>
#include <Raindrop/Graphics/PhysicalDevice.hpp>
#include <Raindrop/Graphics/Device.hpp>
#include <Raindrop/Graphics/Swapchain.hpp>
#include <Raindrop/Graphics/GraphicsPipeline.hpp>
#include <Raindrop/Graphics/ImGUI.hpp>
#include <Raindrop/Core/Asset/AssetManager.hpp>
#include <Raindrop/Core/Scene/Entity.hpp>

#include <SDL2/SDL_vulkan.h>
#include <glm/gtc/matrix_transform.hpp>

namespace Raindrop::Graphics{
	Renderer::Renderer(Core::Event::EventManager& eventManager, Core::Asset::AssetManager& assetManager, Core::Registry::Registry& registry, Core::Scene::Scene& scene) : _eventManager{eventManager}, _assetManager{assetManager}, _registry{registry}, _scene{scene}, _interpreter{_registry}{
		el::Logger* customLogger = el::Loggers::getLogger("Engine.Graphics");
		customLogger->configurations()->set(el::Level::Global, el::ConfigurationType::Format, "%datetime %level [%logger]: %msg");

		
		CLOG(INFO, "Engine.Graphics") << "Creating renderer ...";
		createInstance();
		createPhysicalDeviceManager();

		createWindow();
		createSurface();
		createDevice();
		registerFactories();
		createSwapchain();
		createGraphicsCommandPool();
		createTransfertCommandPool();
		createGraphicsCommandBuffers();
		CLOG(INFO, "Engine.Graphics") << "Created renderer with success !";

		_gui = std::make_unique<ImGUI>(*this);
	}

	Renderer::~Renderer(){
		CLOG(INFO, "Engine.Graphics") << "Destroying renderer ...";
		_device->waitIdle();
		_gui.reset();
		eraseFactories();
		_swapchain.reset();
		if (_surface) vkDestroySurfaceKHR(_instance->get(), _surface, nullptr);
		destroyGraphicsCommandBuffers();
		destroyGraphicsCommandPool();
		destroyTransfertCommandPool();

		_device.reset();
		_physicalDeviceManager.reset();
		_instance.reset();
		CLOG(INFO, "Engine.Graphics") << "Destroyed renderer with success !";
	}

	void drawEntity(Core::Scene::Entity entity, VkPipelineLayout layout, VkCommandBuffer commandBuffer){

		auto& transform = entity.transform();
		PushConstant p;
		p.viewTransform = glm::inverse(glm::scale(glm::mat4(1.f), {1080.f, 720.f, 1.f}));
		p.localTransform = glm::translate(glm::mat4(1.f), transform.translation) * glm::rotate(glm::mat4(1.f), 3.14f, transform.rotation) * glm::scale(glm::mat4(1.f), transform.scale);
		vkCmdPushConstants(commandBuffer, layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstant), &p);
		vkCmdDraw(commandBuffer, 6, 1, 0, 0);

		for (auto c : entity){
			drawEntity(c, layout, commandBuffer);
		}
	}

	void Renderer::openGUI(const std::filesystem::path& path){
		_interpreter.parse(path);
	}

	void Renderer::update(){
		_window->events(_gui.get());

		static Core::Scene::EntityID selectedEntity = Core::Scene::INVALID_ENTITY_ID;

		VkCommandBuffer commandBuffer = beginFrame();
		if (commandBuffer){
			_gui->newFrame();
			_swapchain->beginRenderPass(commandBuffer);

			auto weak_pipeline = _registry["Pipeline"].as<std::weak_ptr<Raindrop::Core::Asset::Asset>>();
			if (auto pipeline = std::static_pointer_cast<GraphicsPipeline>(weak_pipeline.lock())){
				pipeline->bind(commandBuffer);
				drawEntity(Core::Scene::Entity(_scene.root(), &_scene), pipeline->layout(), commandBuffer);
			}

			_interpreter.update();

			// The GUI should be rendered at the end.
			_gui->render(commandBuffer);
			_swapchain->endRenderPass(commandBuffer);
			endFrame();
		}
	}

	void Renderer::createSurface(){
		CLOG(INFO, "Engine.Graphics") << "Creating renderer vulkan surface ...";
		SDL_Vulkan_CreateSurface(_window->get(), _instance->get(), &_surface);
		CLOG(INFO, "Engine.Graphics") << "Created renderer vulkan surface with success !";
	}

	void Renderer::createInstance(){
		CLOG(INFO, "Engine.Graphics") << "Creating renderer vulkan instance ...";
		Builders::InstanceBuilder builder;

		builder.setEngineName("Raindrop");
		builder.setApplicationName("Raindrop::Graphics");

		builder.setEngineVersion(0, 0, 1, 0);
		builder.setApplicationVersion(0, 0, 1, 0);
		builder.setVulkanAPIVersion(VK_API_VERSION_1_3);
		builder.requireGraphicsExtensions();
		builder.requireValidationLayer("VK_LAYER_KHRONOS_validation");

		_instance = builder.build();
		CLOG(INFO, "Engine.Graphics") << "created Renderer vulkan instance created with success !";
	}

	void Renderer::createPhysicalDeviceManager(){
		CLOG(INFO, "Engine.Graphics") << "Creating renderer physical device manager ...";
		_physicalDeviceManager = std::make_shared<PhysicalDeviceManager>(_instance);
		CLOG(INFO, "Engine.Graphics") << "created Renderer vulkan instance created with success !";
	}

	void Renderer::createDevice(){
		CLOG(INFO, "Engine.Graphics") << "Creating renderer device ...";
		Builders::DeviceBuilder builder;

		builder.setPhysicalDevice(findSuitablePhysicalDevice());
		builder.requireExtension(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

		_device = builder.build();

		_graphicsQueue = _device->families()[_device->graphicsFamily()].queues[0];
		_transfertQueue = _device->families()[_device->transfertFamily()].queues[0];
		_presentQueue = _device->families()[_device->presentFamily(_surface)].queues[0];
		CLOG(INFO, "Engine.Graphics") << "Created renderer vulkan device with success !";
	}

	std::shared_ptr<PhysicalDevice> Renderer::findSuitablePhysicalDevice(){
		for (auto& d : _physicalDeviceManager->devices()){
			auto swapchainSupport = d->getSwapchainSupport(_surface);
			if (swapchainSupport.supported) return d;
		}
		return nullptr;
	}

	void Renderer::createWindow(){
		CLOG(INFO, "Engine.Graphics") << "Creating renderer window ...";
		_window = std::make_shared<Window>(_eventManager);
		CLOG(INFO, "Engine.Graphics") << "Created renderer window with success !";
	}

	void Renderer::createSwapchain(){
		CLOG(INFO, "Engine.Graphics") << "Creating renderer swapchain ...";
		auto size = _window->getSize();
		_swapchain = std::make_unique<Swapchain>(_device, _surface, VkExtent2D{size.x, size.y}, _registry);
		_swapchain->setGraphicsQueue(_graphicsQueue);
		_swapchain->setPresentQueue(_presentQueue);
		CLOG(INFO, "Engine.Graphics") << "Created renderer swapchain with success !";
	}

	std::shared_ptr<Instance> Renderer::instance() const{
		return _instance;
	}

	std::shared_ptr<Device> Renderer::device() const{
		return _device;
	}

	void Renderer::createGraphicsCommandPool(){
		VkCommandPoolCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		info.queueFamilyIndex = _device->graphicsFamily();
		info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

		if (vkCreateCommandPool(_device->get(), &info, nullptr, &_graphicsCommandPool) != VK_SUCCESS){
			throw std::runtime_error("failed to create vulkan graphics command pool");
		}
	}

	void Renderer::createGraphicsCommandBuffers(){
		uint32_t count = _swapchain->frameCount();
		_graphicsCommandBuffers.resize(count);

		VkCommandBufferAllocateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		info.commandPool = _graphicsCommandPool;
		info.commandBufferCount = count;

		if (vkAllocateCommandBuffers(_device->get(), &info, _graphicsCommandBuffers.data()) != VK_SUCCESS){
			throw std::runtime_error("failed to allocate vulkan graphics command buffers");
		}
	}

	VkCommandBuffer Renderer::beginFrame(){
		VkResult result = _swapchain->acquireNextImage();
		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			auto size = _window->getSize();
			_swapchain->setExtent(VkExtent2D{size.x, size.y});
			_swapchain->rebuildSwapchain();
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
		VkCommandBuffer commandBuffer = getCurrentGraphicsCommandBuffer();

		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
			throw std::runtime_error("failed to record command buffer");
		
		VkResult result = _swapchain->submitCommandBuffer(&commandBuffer);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || _window->resized()){
			auto size = _window->getSize();
			_swapchain->setExtent(VkExtent2D{size.x, size.y});
			_swapchain->rebuildSwapchain();
		}

		if (result != VK_SUCCESS){
			throw std::runtime_error("failed to submit the command buffer");
		}
	}

	VkCommandBuffer Renderer::getCurrentGraphicsCommandBuffer(){
		return _graphicsCommandBuffers[_swapchain->currentFrame()];
	}

	void Renderer::destroyGraphicsCommandBuffers(){
		vkFreeCommandBuffers(_device->get(), _graphicsCommandPool, _graphicsCommandBuffers.size(), _graphicsCommandBuffers.data());
	}

	void Renderer::destroyGraphicsCommandPool(){
		vkDestroyCommandPool(_device->get(), _graphicsCommandPool, nullptr);
	}

	void Renderer::registerFactories(){
		CLOG(INFO, "Engine.Graphics") << "registering renderer asset factories ...";
		registerShaderFactory();
		registerGraphicsPipelineFactory();
		CLOG(INFO, "Engine.Graphics") << "registred renderer asset factories with success !";
	}

	void Renderer::registerShaderFactory(){
		_shaderFactory = std::make_shared<Factory::ShaderFactory>(_device);
		_assetManager.linkFactory(".spv", _shaderFactory);
	}

	void Renderer::registerGraphicsPipelineFactory(){
		_graphicsPipelineFactory = std::make_unique<Factory::GraphicsPipelineFactory>(_device, _assetManager, _registry, nullptr);
		_graphicsPipelineFactory->registerExtensions(_graphicsPipelineFactory);
	}

	void Renderer::eraseFactories(){
		_shaderFactory.reset();
		_graphicsPipelineFactory.reset();
	}

	void Renderer::createTransfertCommandPool(){
		VkCommandPoolCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		info.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
		info.queueFamilyIndex = _device->transfertFamily();

		if (vkCreateCommandPool(_device->get(), &info, VK_NULL_HANDLE, &_transfertCommandPool) != VK_SUCCESS){
			throw std::runtime_error("Failed to create transfert command pool");
		}
	}
	
	VkCommandBuffer Renderer::beginSingleUseTransfertCommandBuffer(){
		VkCommandBufferAllocateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		info.commandBufferCount = 1;
		info.commandPool = _transfertCommandPool;

		VkCommandBuffer commandBuffer;

		if (vkAllocateCommandBuffers(_device->get(), &info, &commandBuffer) != VK_SUCCESS){
			throw std::runtime_error("Failed to allocate transfert command buffer");
		}

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS){
			throw std::runtime_error("Failed to begin transfert command buffer");
		}

		return commandBuffer;
	}

	void Renderer::endSingleUseTransfertCommandBuffer(VkCommandBuffer commandBuffer){
		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS){
			throw std::runtime_error("Failed to end transfert command buffer");
		}

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(_transfertQueue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(_transfertQueue);

		vkFreeCommandBuffers(_device->get(), _transfertCommandPool, 1, &commandBuffer);
	}

	void Renderer::destroyTransfertCommandPool(){
		vkDestroyCommandPool(_device->get(), _transfertCommandPool, nullptr);
	}

}