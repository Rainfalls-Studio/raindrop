#include <imgui/backends/imgui_impl_vulkan.h> 
#include <imgui/backends/imgui_impl_sdl2.h>

#include <Raindrop/Graphics/ImGUI.hpp>
#include <Raindrop/Graphics/Renderer.hpp>
#include <Raindrop/Graphics/Window.hpp>
#include <Raindrop/Graphics/Device.hpp>
#include <Raindrop/Graphics/PhysicalDevice.hpp>
#include <Raindrop/Graphics/Swapchain.hpp>
#include <SDL2/SDL_vulkan.h>

namespace Raindrop::Graphics{
	ImGUI::ImGUI(Renderer& renderer){
		el::Logger* customLogger = el::Loggers::getLogger("Engine.Graphics.GUI");
		customLogger->configurations()->set(el::Level::Global, el::ConfigurationType::Format, "%datetime %level [%logger]: %msg");

		CLOG(INFO, "Engine.Graphics.GUI") << "Loading IMGUI...";

		_device = renderer._device->get();
		auto window = renderer._window->get();
		auto instance = renderer._instance->get();
		auto physicalDevice = renderer._device->physicalDevice()->get();
		auto device = renderer._device->get();
		auto renderPass = renderer._swapchain->renderPass();
		auto graphicsQueue = renderer._graphicsQueue;

		VkDescriptorPoolSize pool_sizes[] ={
			{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
		};

		VkDescriptorPoolCreateInfo pool_info = {};
		pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		pool_info.maxSets = 1000;
		pool_info.poolSizeCount = std::size(pool_sizes);
		pool_info.pPoolSizes = pool_sizes;

		if (vkCreateDescriptorPool(device, &pool_info, nullptr, &_imguiPool) != VK_SUCCESS){
			CLOG(ERROR, "Engine.Graphics.GUI") << "Failed to create IMGUI descriptor pool";
			throw std::runtime_error("failed to create GUI descriptor pool");
		}

		//this initializes the core structures of imgui
		ImGui::CreateContext();
		ImGui_ImplSDL2_InitForVulkan(window);

		//this initializes imgui for Vulkan
		ImGui_ImplVulkan_InitInfo init_info = {};
		init_info.Instance = instance;
		init_info.PhysicalDevice = physicalDevice;
		init_info.Device = device;
		init_info.Queue = graphicsQueue;
		init_info.DescriptorPool = _imguiPool;
		init_info.MinImageCount = renderer._swapchain->frameCount();
		init_info.ImageCount = renderer._swapchain->frameCount();
		init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

		ImGui_ImplVulkan_Init(&init_info, renderPass);

		{
			VkCommandBuffer cmd = renderer.beginSingleUseTransfertCommandBuffer();
			ImGui_ImplVulkan_CreateFontsTexture(cmd);
			renderer.endSingleUseTransfertCommandBuffer(cmd);
		}

		ImGui_ImplVulkan_DestroyFontUploadObjects();
		
		CLOG(INFO, "Engine.Graphics.GUI") << "Loaded IMGUI with success!";
	}

	ImGUI::~ImGUI(){
		CLOG(INFO, "Engine.Graphics.GUI") << "Destroying IMGUI...";
		vkDestroyDescriptorPool(_device, _imguiPool, nullptr);
		ImGui_ImplVulkan_Shutdown();
		CLOG(INFO, "Engine.Graphics.GUI") << "IMGUI destroyed with success !";
	}

	void ImGUI::newFrame(){
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
	}

	void ImGUI::render(VkCommandBuffer commandBuffer){
		ImGui::Render();
		ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);
	}

	void ImGUI::event(SDL_Event* e){
		ImGui_ImplSDL2_ProcessEvent(e);
	}
}