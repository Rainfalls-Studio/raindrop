#pragma once

#include <Raindrop/pch.pch>
#include "DescriptorSet.hpp"
#include "DescriptorSetLayout.hpp"

namespace Raindrop::Graphics{
	class Context;

	namespace Core::Descriptor{
		class DescriptorPool{
			public:
				static std::shared_ptr<DescriptorPool> create(Context& context);
				
				DescriptorPool() noexcept;
				~DescriptorPool();

				DescriptorPool(const DescriptorPool&) = delete;
				DescriptorPool& operator=(const DescriptorPool&) = delete;

				DescriptorPool(DescriptorPool&& other);
				DescriptorPool& operator=(DescriptorPool&& other);

				friend void swap(DescriptorPool& A, DescriptorPool& B);

				void prepare(Context& context);
				void initialize();
				void release();

				DescriptorPool& setFlags(const VkDescriptorPoolCreateFlags& flags);
				DescriptorPool& setMaxSets(const std::uint32_t& max);
				DescriptorPool& allocate(const VkDescriptorType& type, const std::uint32_t& count);

				const VkDescriptorPool& get() const noexcept;

				std::vector<DescriptorSet> allocate(const std::vector<DescriptorSetLayout>& layouts);
				void reset(const VkDescriptorPoolResetFlags& flags = 0);
				void free(const std::vector<DescriptorSet>& sets);

			private:
				struct BuildInfo{
					VkDescriptorPoolCreateInfo info;
					std::vector<VkDescriptorPoolSize> sizes;
					
					BuildInfo();
				};

				Context* _context;
				VkDescriptorPool _pool;

				std::unique_ptr<BuildInfo> _info;
				BuildInfo& getInfo();

				void checkInitialize();
		};
	}
}