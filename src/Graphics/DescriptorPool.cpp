#include <Raindrop/Graphics/DescriptorPool.hpp>
#include <Raindrop/Graphics/GraphicsContext.hpp>

namespace Raindrop::Graphics{
	DescriptorPool::DescriptorPool(GraphicsContext& context, VkDescriptorPoolCreateInfo& info) : _context{context}{
		el::Logger* customLogger = el::Loggers::getLogger("Engine.Graphics.DescriptorPool");
		customLogger->configurations()->set(el::Level::Global, el::ConfigurationType::Format, "%datetime %level [%logger]: %msg");

		if (vkCreateDescriptorPool(_context.device.get(), &info, _context.allocationCallbacks, &_pool) != VK_SUCCESS){
			CLOG(ERROR, "Engine.Graphics.DescriptorPool") << "Failed to create descriptor pool";
			throw std::runtime_error("Failed to create descriptorPool");
		}
	}

	DescriptorPool::~DescriptorPool(){
		if (_pool) vkDestroyDescriptorPool(_context.device.get(), _pool, _context.allocationCallbacks);
	}

	VkDescriptorPool DescriptorPool::get() const{
		return _pool;
	}
}