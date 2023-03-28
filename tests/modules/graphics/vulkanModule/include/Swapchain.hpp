#ifndef __VULKANMODULE_SWAPCHAIN_HPP__
#define __VULKANMODULE_SWAPCHAIN_HPP__

#include "core.hpp"
#include "device/Device.hpp"

class Swapchain{
	public:
		Swapchain(GfxAllocator& allocator, VkSurfaceKHR surface, Device &device, uint32 familyIndex);
		Swapchain(GfxAllocator& allocator, VkSurfaceKHR surface, Device &device, Swapchain* previous);
		~Swapchain();

		void setExtent(VkExtent2D extent);
		void setBufferCount(uint32 bufferCount);
		void enableVSync(bool enable = true);
		void requireDepthBuffer(bool require = true);
		void setRenderQuality(maths::vec2<float32> quality);
		void setRequiredColorFormat(VkFormat colorFormat);
		void setRequiredDepthFormat(VkFormat depthFormat);
		void setRequiredColorSpace(VkColorSpaceKHR colorSpace);

		void init();

		VkRenderPass renderPass() const;
		bool vsync() const;
		bool loaded() const;
		uint32 bufferCount() const;
		uint32 currentBuffer() const;
		VkFormat depthFormat() const;
		VkFormat colorFormat() const;
		maths::vec2<float32> renderQuality() const;
		maths::vec4<float32> clearColor() const;

		void setClearColor(maths::vec4<float32> color);

		void beginRenderPass(VkCommandBuffer commandBuffer);
		void endRenderPass(VkCommandBuffer commandBuffer);
		VkResult submitCommandBuffer(VkCommandBuffer* buffers);
		VkResult acquireNextImage();
		
	private:
		struct Frame{
			VkFramebuffer _framebuffer;
			VkImage _depth;
			VkImage _image;
			VkDeviceMemory _imageMemory;
			VkDeviceMemory _depthMemory;
			VkImageView _imageView;
			VkImageView _depthView;
			VkSemaphore _imageAvailable;
			VkSemaphore _imageFinished;
			VkFence _inFlightFence;
			VkFence _imageInFlight;

			Frame();
		};

		GfxAllocator& _allocator;
		Device& _device;
		Swapchain* _previous;
		VkSwapchainKHR _swapchain;
		VkSurfaceKHR _surface;

		VkFormat _colorFormat;
		VkFormat _depthFormat;
		VkColorSpaceKHR _colorSpace;
		VkExtent2D _extent;
		uint32 _currentFrame;
  		VkRenderPass _renderPass;
		Array<Frame> _frames;
		VkPresentModeKHR _presentMode;

		uint32 _bufferCount;
		bool _vsync;
		bool _depthBufferRequired;
		bool _loaded;
		maths::vec2<float32> _renderQuality;
		maths::vec4<float32> _clearColor;

		void defaultSettings();
		VkFormat findSupportedDepthFormat();
		void updatePresentMode(Array<VkPresentModeKHR>& presentModes);

		uint32 getGraphicsFamilyIndex();
		uint32 getPresentFamilyIndex();

		VkAttachmentDescription getDepthAttachment();
		VkAttachmentDescription getColorAttachment();

		bool createSwapchain();
		bool createImageViews();
		bool createDepthImages();
		bool createDepthImageView();
		bool createRenderPass();
		bool createDepthResources();
		bool createFramebuffers();
		bool createSyncObjects();
};

#endif