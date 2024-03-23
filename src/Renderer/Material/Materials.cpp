#include <Raindrop/Renderer/Material/Materials.hpp>
#include <Raindrop/Renderer/Material/Context.hpp>

#include <spdlog/spdlog.h>

namespace Raindrop::Renderer::Material{
	Materials::Materials(::Raindrop::Renderer::Context& context) : _context{nullptr}{
		_context = new Context(context);

		Material placeholder(context);
		placeholder.properties = {
			.diffuseColor = glm::vec3(0.f),
			.specularColor = glm::vec3(0.f),
			.shininess = 0.f,
		};

		assert(registerMaterial(placeholder) == PLACEHOLDER_MATERIAL && "Failed to create the placeholder material");
	}

	Materials::~Materials(){
		if (_context != nullptr){
			delete _context;
			_context = nullptr;
		}
	}

	MaterialID Materials::registerMaterial(const Material& material){
		MaterialID ID = _context->registry.registerMaterial();
		VkDescriptorSet set = _context->pool.getSet(static_cast<std::size_t>(ID));

		_context->registry.updateMaterial(ID, Registry::MaterialPair(material, set));
		updateMaterial(ID, material);
		return ID;
	}

	void Materials::unregisterMaterial(const MaterialID& ID){
		_context->registry.unregisterMaterial(ID);
	}

	Material& Materials::getMaterial(const MaterialID& ID){
		const auto& pair = _context->registry.get(ID);
		if (pair){
			return pair->material;
		} else {
			return getMaterial(PLACEHOLDER_MATERIAL);
		}
	}

	VkDescriptorSet Materials::getDescriptorSet(const MaterialID& ID){
		const auto& pair = _context->registry.get(ID);
		if (pair){
			return pair->set;
		} else {
			return getDescriptorSet(PLACEHOLDER_MATERIAL);
		}
	}

	void Materials::updateMaterial(const MaterialID& ID){
		updateMaterial(ID, getMaterial(ID));
	}

	void Materials::updateMaterial(const MaterialID& ID, const Material& material){
		auto& device = _context->renderer.device;
		_context->buffer.write(ID, material.properties);

		auto set = getDescriptorSet(ID);

		VkDescriptorImageInfo imageInfos[] = {
			material.textures.getDiffuse().info()
		};

		VkDescriptorBufferInfo bufferInfo[] = {
			{
				.buffer = _context->buffer.get(),
				.offset = static_cast<VkDeviceSize>(ID * sizeof(Material::Properties)),
				.range = static_cast<VkDeviceSize>(sizeof(Material::Properties))
			}
		};

		VkWriteDescriptorSet writes[] = {
			{
				.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
				.dstSet = set,
				.dstBinding = 1,
				.descriptorCount = 1,
				.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
				.pImageInfo = imageInfos
			},
			{
				.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
				.dstSet = set,
				.dstBinding = 0,
				.descriptorCount = 1,
				.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
				.pBufferInfo = bufferInfo
			}
		};

		vkUpdateDescriptorSets(
			device.get(),
			sizeof(writes) / sizeof(writes[0]),
			writes,
			0,
			nullptr
		);
	}

	SetLayout& Materials::layout() const{
		return _context->layout;
	}

}