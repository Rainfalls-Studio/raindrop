#pragma once

#include <Raindrop/pch.pch>

namespace Raindrop::Graphics{
	class Context;

	namespace Core::Descriptor{
		class DescriptorSetLayout{
			public:
				class Binding{
					public:
						Binding(VkDescriptorSetLayoutBinding& info, std::vector<VkSampler>& immutableSampler) noexcept;

						Binding& setBinding(const std::uint32_t& binding) noexcept;
						Binding& setDescriptorType(const VkDescriptorType& type) noexcept;
						Binding& setDescriptorCount(const std::uint32_t &count) noexcept;
						Binding& setShaderStage(const VkShaderStageFlags& stages) noexcept;
						Binding& setImmutableSamplers(const std::vector<VkSampler>& samplers);

						VkDescriptorSetLayoutBinding& get() noexcept;

					private:
						VkDescriptorSetLayoutBinding& _info;
						std::vector<VkSampler>& _immutableSampler;
				};

				static std::shared_ptr<DescriptorSetLayout> create(Context& context);

				DescriptorSetLayout() noexcept;
				~DescriptorSetLayout();

				DescriptorSetLayout(const DescriptorSetLayout&) = delete;
				DescriptorSetLayout& operator=(const DescriptorSetLayout&) = delete;

				DescriptorSetLayout(DescriptorSetLayout&& other);
				DescriptorSetLayout& operator=(DescriptorSetLayout&& other);

				friend void swap(DescriptorSetLayout& A, DescriptorSetLayout& B);

				void prepare(Context& context);
				void initialize();
				void release();

				DescriptorSetLayout& setFlags(const VkDescriptorSetLayoutCreateFlags& flags) noexcept;
				Binding addBinding();

				const VkDescriptorSetLayout& get() const noexcept;
				VkDescriptorSetLayout& get() noexcept;

			private:
				Context* _context;
				VkDescriptorSetLayout _layout;

				struct BuildInfo{
					VkDescriptorSetLayoutCreateFlags flags;
					std::vector<VkDescriptorSetLayoutBinding> bindings;
					std::vector<VkSampler> immutableSamplers;
				};

				std::unique_ptr<BuildInfo> _info;

				BuildInfo& getInfo();
		};
	}
}