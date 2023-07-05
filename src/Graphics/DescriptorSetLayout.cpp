#include <Raindrop/Graphics/DescriptorSetLayout.hpp>
#include <Raindrop/Graphics/GraphicsContext.hpp>

namespace Raindrop::Graphics{
	DescriptorSetLayout::DescriptorSetLayout(GraphicsContext& context, VkDescriptorSetLayoutCreateInfo& info) : _context{context}{
		el::Logger* customLogger = el::Loggers::getLogger("Engine.Graphics.DescriptorSetLayout");
		customLogger->configurations()->set(el::Level::Global, el::ConfigurationType::Format, "%datetime %level [%logger]: %msg");

		if (vkCreateDescriptorSetLayout(_context.device.get(), &info, _context.allocationCallbacks, &_layout) != VK_SUCCESS){
			CLOG(ERROR, "Engine.Graphics.DescriptorSetLayout") << "Failed to create descriptor set layout";
			throw std::runtime_error("Failed to create descriptor set layout");
		}
	}

	DescriptorSetLayout::~DescriptorSetLayout(){
		if (_layout) vkDestroyDescriptorSetLayout(_context.device.get(), _layout, _context.allocationCallbacks);
	}
}