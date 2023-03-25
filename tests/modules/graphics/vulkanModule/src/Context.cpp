#include "Context.hpp"

Context::Context(Device& device, GfxAllocator& allocator, Window& window) : _device{device}, _allocator{allocator}, _window{window}, _commandBuffers{_allocator.get()}{
	RAINDROP_profile_function();

	_surface = _window.createSurface(_device.instance().get());

	uint32 familyIndex = _device.physicalDevice().findFamily(VK_QUEUE_GRAPHICS_BIT, true, _surface);
	if (familyIndex == (uint32)-1){
		RAINDROP_log(ERROR, GRAPHICS, "failed to find a suitable vulkan queue family for the swapchain");
		return;
	}
	
	_swapchain = allocateNew<Swapchain>(allocator.get(), _allocator, _surface, _device, familyIndex);
	_swapchain->setExtent(getWindowExtent());
	_swapchain->init();

	if (!_swapchain->loaded()){
		RAINDROP_log(ERROR, GRAPHICS, "failed to create vulkan context swapchain");
		return;
	}

	_commandPool = _device.getCommandPool(VK_QUEUE_GRAPHICS_BIT, familyIndex);
	_bufferCount = _swapchain->bufferCount();

	if (!createCommandBuffers()){
		RAINDROP_log(ERROR, GRAPHICS, "failed to create vulkan context command buffers");
		return;
	}

	_frameStarted = false;
}

Context::~Context(){
	RAINDROP_profile_function();
	vkDestroyRenderPass(_device.get(), _swapchain->renderPass(), _allocator.callback());
	vkFreeCommandBuffers(_device.get(), _commandPool, _bufferCount, _commandBuffers.data());
	deallocateDelete(_allocator.get(), *_swapchain);
}

bool Context::begin(){
	RAINDROP_profile_function();

	VkResult result = _swapchain->acquireNextImage();
	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		Swapchain* newSwapchain = allocateNew<Swapchain>(_allocator.get(), _allocator, _surface, _device, _swapchain);
		newSwapchain->init();

		if (!newSwapchain->loaded()){
			return false;
		}

		deallocateDelete(_allocator.get(), *_swapchain);
		_swapchain = newSwapchain;
		return false;
	}

	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		RAINDROP_log(ERROR, GRAPHICS, "failed to acquire vulkan swapchain image");
		return false;
	}
	
	VkCommandBuffer commandBuffer = getCurrentCommandBuffer();
	VkCommandBufferBeginInfo info{};
	info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	
	result = vkBeginCommandBuffer(commandBuffer, &info);
	if (result != VK_SUCCESS){
		RAINDROP_log(ERROR, GRAPHICS, "failed to begin vulkan context command buffer");
		return false;
	}

	_frameStarted = true;
	return true;
}

void Context::end(){
	RAINDROP_profile_function();
	RAINDROP_assert(_frameStarted);
	
	VkCommandBuffer commandBuffer = getCurrentCommandBuffer();
	VkResult result = vkEndCommandBuffer(commandBuffer);
	if (result != VK_SUCCESS){
		RAINDROP_log(ERROR, GRAPHICS, "failed to end vulkan context command buffer");
		return;
	}

	_swapchain->submitCommandBuffer(&commandBuffer);

	_frameStarted = false;
}

void Context::setClearColor(Core::Maths::vec4<float32> color){
	RAINDROP_profile_function();
	_swapchain->setClearColor(color);
}

void Context::enableVSync(bool enable){
	RAINDROP_profile_function();

	if (_swapchain->vsync() != enable){
		Swapchain* newSwapchain = allocateNew<Swapchain>(_allocator.get(), _allocator, _surface, _device, _swapchain);
		newSwapchain->setExtent(getWindowExtent());
		newSwapchain->enableVSync(enable);
		newSwapchain->init();

		if (!newSwapchain->loaded()){
			RAINDROP_log(ERROR, GRAPHICS, "failed to recreate a vulkan swapchain with vsync %s", enable ? "enable" : "disable");
			deallocateDelete(_allocator.get(), *newSwapchain);
			return;
		}

		deallocateDelete(_allocator.get(), *_swapchain);
		_swapchain = newSwapchain;
	}
}

bool Context::isVSyncEnabled() const{
	RAINDROP_profile_function();
	return _swapchain->vsync();
}

VkExtent2D Context::getWindowExtent(){
	Maths::vec2<uint32> size = _window.getSize();
	return VkExtent2D{size.x, size.y};
}

bool Context::createCommandBuffers(){
	RAINDROP_profile_function();
	_commandBuffers.resize(_bufferCount);

	VkCommandBufferAllocateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	info.commandPool = _commandPool;
	info.commandBufferCount = _bufferCount;

	VkResult result = vkAllocateCommandBuffers(_device.get(), &info, _commandBuffers.data());
	if (result != VK_SUCCESS){
		RAINDROP_log(ERROR, GRAPHICS, "failed to allocate %d vulkan command buffers", _bufferCount);
		return false;
	}

	return true;
}

VkCommandBuffer Context::getCurrentCommandBuffer() const{
	return _commandBuffers[_swapchain->currentBuffer()];
}

const Swapchain& Context::swapchain() const{
	return *_swapchain;
}

VkRenderPass Context::swapchainRenderPass() const{
	return _swapchain->renderPass();
}