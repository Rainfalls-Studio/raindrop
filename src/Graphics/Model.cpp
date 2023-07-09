#include <Raindrop/Graphics/Model.hpp>
#include <Raindrop/Graphics/Buffer.hpp>
#include <Raindrop/Graphics/Device.hpp>
#include <Raindrop/Graphics/GraphicsContext.hpp>

namespace Raindrop::Graphics{
	Model::Model(GraphicsContext& context, Builders::ModelBuilder& builder) : _context{context}{
		createVertexBuffer(builder);
		createIndexBuffer(builder);
	}

	Model::~Model(){
		_vertexBuffer.reset();
		_indexBuffer.reset();
	}
	
	void Model::createVertexBuffer(Builders::ModelBuilder& builder){
		auto& vertices = builder.vertices();
		if (vertices.empty()) return;

		_vertexCount = vertices.size();

		Buffer staginBuffer(_context);
		staginBuffer.allocate(sizeof(Vertex) * vertices.size(), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
		staginBuffer.map();
		staginBuffer.writeToBuffer((void*)vertices.data());
		staginBuffer.flush();

		_vertexBuffer = std::make_unique<Buffer>(_context);
		_vertexBuffer->allocate(sizeof(Vertex) * vertices.size(), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		VkCommandBuffer commandBuffer = _context.transfertCommandPool.beginSingleTime();

		VkBufferCopy region{};
		region.dstOffset = 0;
		region.srcOffset = 0;
		region.size = sizeof(Vertex) * vertices.size();

		vkCmdCopyBuffer(commandBuffer, staginBuffer.get(), _vertexBuffer->get(), 1, &region);

		_context.transfertCommandPool.endSingleTime(commandBuffer);
	}

	void Model::createIndexBuffer(Builders::ModelBuilder& builder){
		auto& indices = builder.indices();
		if (indices.empty()) return;

		_indexCount = indices.size();

		Buffer staginBuffer(_context);
		staginBuffer.allocate(sizeof(uint32_t) * indices.size(), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
		staginBuffer.map();
		staginBuffer.writeToBuffer((void*)indices.data());
		staginBuffer.flush();

		_indexBuffer = std::make_unique<Buffer>(_context);
		_indexBuffer->allocate(sizeof(uint32_t) * indices.size(), VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		VkCommandBuffer commandBuffer = _context.transfertCommandPool.beginSingleTime();

		VkBufferCopy region{};
		region.dstOffset = 0;
		region.srcOffset = 0;
		region.size = sizeof(uint32_t) * indices.size();

		vkCmdCopyBuffer(commandBuffer, staginBuffer.get(), _indexBuffer->get(), 1, &region);

		_context.transfertCommandPool.endSingleTime(commandBuffer);
	}

	void Model::draw(VkCommandBuffer commandBuffer){
		VkDeviceSize offsets[] = {0};
		VkBuffer buffers[] = {_vertexBuffer->get()};
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
		if (_indexBuffer){
			vkCmdBindIndexBuffer(commandBuffer, _indexBuffer->get(), 0, VK_INDEX_TYPE_UINT32);
			vkCmdDrawIndexed(commandBuffer, _indexCount, 1, 0, 0, 0);
		} else {
			vkCmdDraw(commandBuffer, _vertexCount, 1, 0, 0);
		}
	}
}
