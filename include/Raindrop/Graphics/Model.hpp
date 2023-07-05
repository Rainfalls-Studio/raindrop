#ifndef __RAINDROP_GRAPHICS_MODEL_HPP__
#define __RAINDROP_GRAPHICS_MODEL_HPP__

#include <Raindrop/Graphics/common.hpp>
#include <Raindrop/Graphics/builders/ModelBuilder.hpp>

namespace Raindrop::Graphics{
	class Model : public Core::Asset::Asset{
		public:
			Model(GraphicsContext& context, Builders::ModelBuilder& builder);
			~Model();

			void draw(VkCommandBuffer commandBuffer);

		public:
			GraphicsContext& _context;
			std::unique_ptr<Buffer> _vertexBuffer;
			std::unique_ptr<Buffer> _indexBuffer;

			uint32_t _indexCount = 0;
			uint32_t _vertexCount = 0;

			void createVertexBuffer(Builders::ModelBuilder& builder);
			void createIndexBuffer(Builders::ModelBuilder& builder);
	};
}

#endif