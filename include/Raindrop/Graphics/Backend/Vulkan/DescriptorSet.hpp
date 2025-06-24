#pragma once

#include <Raindrop/pch.pch>

namespace Raindrop::Graphics{
	class Context;

	namespace Core::Descriptor{
		class DescriptorSet{
			public:
				DescriptorSet() noexcept;
				DescriptorSet(Context& context, const VkDescriptorSet& set) noexcept;
				~DescriptorSet();

				const VkDescriptorSet& get() const noexcept;
			
			private:
				Context* _context;
				VkDescriptorSet _set;
		};
	}
}