#pragma once

#include "types.hpp"
#include "pch.pch"
#include "Utils.hpp"
#include "Formats.hpp"
#include <Raindrop/Object.hpp>

namespace Raindrop::Graphics{
	class VertexLayout : public Object{
		public:
			enum AttributeUsage{
				POSITION = 1 << 0,
				NORMAL = 1 << 1,
				COLOR = 1 << 2,
				UV = 1 << 3,
				TANGENT = 1 << 4,
				BITANGENT = 1 << 5,

				OTHER = 1 << 6 
			};

			using UsageFlags = std::uint32_t;

			static constexpr std::uint32_t BINDING_AUTO = UINT32_MAX;
			static constexpr std::uint32_t LOCATION_AUTO = UINT32_MAX;

			struct AttributeInfo{
				VkFormat format;
				std::uint32_t offset;
				std::uint32_t binding;
				std::uint32_t location;
				std::string name;
				AttributeUsage usage;
				Formats::ComponentSwizzle componentMapping;
			};

			struct BindingInfo{
				std::uint32_t binding;
				std::uint32_t stride;
				VkMemoryPropertyFlags memoryProperties;
				std::string name;
				VkVertexInputRate rate;

				std::unordered_map<std::string, AttributeInfo>  attributes;
				
				/**
				 * @brief Access the attribute named 'name'
				 * 
				 * @param name The name of the attribute to access
				 * @return const AttributeInfo& 
				 */
				const AttributeInfo& operator[](const std::string& name) const;
			};

			class Binding{
				friend class VertexLayout;
				public:	
					Binding(BindingInfo& info) noexcept;
					~Binding() = default;

					/**
					 * @brief Set the wanted memory properties of the binding
					 * 
					 * @param flags The memory properties flags
					 * @return Binding& 
					 */
					Binding& setMemoryProperties(const VkMemoryPropertyFlags& flags);

					/**
					 * @brief Set the input rate of the binding
					 * 
					 * @param rate The binding's input rate
					 * @return Binding& 
					 */
					Binding& setInputRate(const VkVertexInputRate& rate);

					/**
					 * @brief Add a new attribute into the vertex layout binding
					 * 
					 * @param name The name of the attribute. /!\ If the name is already used, the previous data will be overwriten /!\
					 * @param format The format of the attribute, must be a valid vulkan vertex attribute format
					 * @param usage The usage of the attribute. This is used on mesh loading. The data found in the mesh file will be written on the corresponding vertex attribute
					 * @param location The location of the attribute withing the binding, by default will be the next available location.
					 * @param componentMapping The mapping of the components, useful to isolate data as a differnt variable
					 * @return Binding& 
					 */
					Binding& addAttribute(const std::string& name, const VkFormat format, const AttributeUsage& usage = OTHER, const std::uint32_t& location = LOCATION_AUTO, const Formats::ComponentSwizzle& componentMapping = Formats::ComponentSwizzle());

					/**
					 * @brief Add a new attribute into the vertex layout binding
					 * 
					 * @tparam T The type of the vertex attribute. This type is converted into a valid vulkan vertex attribute format.
					 * @param name The name of the attribute. /!\ If the name is already used, the previous data will be overwriten /!\
					 * @param usage The usage of the attribute. This is used on mesh loading. The data found in the mesh file will be written on the corresponding vertex attribute
					 * @param location The location of the attribute withing the binding, by default will be the next available location.
					 * @param componentMapping The mapping of the components, useful to isolate data as a differnt variable
					 * @return Binding& 
					 */
					template<typename T>
					Binding& addAttribute(const std::string& name, const AttributeUsage& usage = OTHER, const std::uint32_t& location = LOCATION_AUTO, const Formats::ComponentSwizzle& componentMapping = Formats::ComponentSwizzle()){
						return addAttribute(name, Utils::typeToFormat<T>(), usage, location, componentMapping);
					}

					/**
					 * @brief Get the binding raw info
					 * 
					 * @return const BindingInfo& 
					 */
					const BindingInfo& get() const noexcept;

				private:
					BindingInfo& _info;
			};

			static std::shared_ptr<VertexLayout> create(Raindrop::Context& context);

			VertexLayout() noexcept;
			~VertexLayout() = default;
			
			/**
			 * @brief Add a binding to the vertex layout.
			 * 
			 * @param name The name of the binding
			 * @param rate the input rate for this binding (per vertex or per instance)
			 * @param memProperties The required memory properties for this binding (eg. host visible for dynamic updates of the buffer)
			 * @param binding the binding index
			 * @return Binding 
			 */
			Binding addBinding(const std::string& name, const VkVertexInputRate& rate = VK_VERTEX_INPUT_RATE_VERTEX, const VkMemoryPropertyFlags& memProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, std::uint32_t binding = BINDING_AUTO);

			/**
			 * @brief Get the bindings as an array of vulkan binding description
			 * 
			 * @return std::vector<VkVertexInputBindingDescription> 
			 */
			std::vector<VkVertexInputBindingDescription> getBindingDescriptions() const;

			/**
			 * @brief Get the attributes as an array of vulkan attribute description
			 * 
			 * @return std::vector<VkVertexInputAttributeDescription> 
			 */
			std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions() const;

			/**
			 * @brief Access the binding named 'name'
			 * 
			 * @param name The name of the binding to access
			 * @return BindingInfo& 
			 */
			BindingInfo& operator[](const std::string& name);

			/**
			 * @brief Access the binding named 'name'
			 * 
			 * @param name The name of the binding to access
			 * @return const BindingInfo& 
			 */
			const BindingInfo& operator[](const std::string& name) const;

			/**
			 * @brief Get the total number of bindings in the vertex layout
			 * 
			 * @return std::size_t 
			 */
			std::size_t getBindingCount() const noexcept;

			/**
			 * @brief Get the bindings as an unordered map mapping the name of the bindings to their data
			 * 
			 * @return const std::unordered_map<std::string, BindingInfo>& 
			 */
			std::unordered_map<std::string, BindingInfo>& getBindings() noexcept;

			/**
			 * @brief Get a vector of all the attributes that have as a usage the inout value
			 * 
			 * @param usage The usage to filter
			 * @return std::vector<AttributeInfo> 
			 */
			std::vector<AttributeInfo> getDedicatedAttributes(const AttributeUsage& usage) const;

		private:
			std::unordered_map<std::string, BindingInfo> _bindings;
	};
}