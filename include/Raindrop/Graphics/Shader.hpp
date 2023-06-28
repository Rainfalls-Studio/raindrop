#ifndef __RAINDROP_GRAPHICS_SHADER_HPP__
#define __RAINDROP_GRAPHICS_SHADER_HPP__

#include <Raindrop/Graphics/common.hpp>

namespace Raindrop::Graphics{
	class Shader : public Core::Asset::Asset{
		public:
			Shader(GraphicsContext& context, const std::vector<char>& code, VkShaderStageFlagBits stage);
			virtual ~Shader() override;

			VkShaderModule get() const;
			VkShaderStageFlagBits stage() const;

		private:
			GraphicsContext& _context;
			VkShaderModule _shader = VK_NULL_HANDLE;
			VkShaderStageFlagBits _stage;

			void createShaderModule(const std::vector<char>& code);
	};
}

#endif