#include "render/vulkan/utils.hpp"

#include <cassert>

namespace rnd::render::vulkan{
	void transitionImageLayout(VkCommandBuffer commandBuffer, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout){

		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;

		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

		barrier.image = image;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = 1;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;

		VkPipelineStageFlags sourceStage;
		VkPipelineStageFlags destinationStage;

		if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL){
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT | VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;

		} else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL){
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;

			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;

		} else if (oldLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL){
			barrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT; 
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			sourceStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;

		} else if (oldLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL){
			barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;

		} else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL){
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		} else {
			throw "unsupported layout transition !";
		}

		vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
	}

	void copyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height){

		VkBufferImageCopy region{};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;

		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;

		region.imageOffset = {0, 0, 0};
		region.imageExtent = {width, height, 1};

		vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
	}

	VkQueueFlagBits familyQueueToVkQueue(QueueFamily family){
		switch (family){
			case FAMILY_COMPUTE: return VK_QUEUE_COMPUTE_BIT;
			case FAMILY_GRAPHIC: return VK_QUEUE_GRAPHICS_BIT;
			case FAMILY_PROTECTED: return VK_QUEUE_PROTECTED_BIT;
			case FAMILY_SPARSE_BINDING: return VK_QUEUE_SPARSE_BINDING_BIT;
			case FAMILY_TRANSFER: return VK_QUEUE_TRANSFER_BIT;
		}
		
		assert(family != FAMILY_PRESENT && "the present queue family cannot be converted into a vulkan queue family");
		assert(false && "inavlid queue family");
		return VK_QUEUE_FLAG_BITS_MAX_ENUM;
	}

	const char* resultToStr(VkResult result){
		switch (result){
			case VK_SUCCESS: return "success";
			case VK_NOT_READY: return "not ready";
			case VK_TIMEOUT: return "timeout";
			case VK_EVENT_SET: return "event set";
			case VK_EVENT_RESET: return "event reset";
			case VK_INCOMPLETE: return "incomplete";
			case VK_ERROR_OUT_OF_HOST_MEMORY: return "ERROR : out of host memory";
			case VK_ERROR_OUT_OF_DEVICE_MEMORY: return "ERROR : out of device memory";
			case VK_ERROR_INITIALIZATION_FAILED: return "ERROR : initialization failed";
			case VK_ERROR_DEVICE_LOST: return "ERROR : device lost";
			case VK_ERROR_MEMORY_MAP_FAILED: return "ERROR : memory map failed";
			case VK_ERROR_LAYER_NOT_PRESENT: return "ERROR : layer not present";
			case VK_ERROR_EXTENSION_NOT_PRESENT: return "ERROR : extension not present";
			case VK_ERROR_FEATURE_NOT_PRESENT: return "ERROR : feature not present";
			case VK_ERROR_INCOMPATIBLE_DRIVER: return "ERROR : incompatible driver";
			case VK_ERROR_TOO_MANY_OBJECTS: return "ERROR : too many objects";
			case VK_ERROR_FORMAT_NOT_SUPPORTED: return "ERROR : format not supported";
			case VK_ERROR_FRAGMENTED_POOL: return "ERROR : fragmented pool";
			case VK_ERROR_UNKNOWN: return "ERROR : unknown";
			case VK_ERROR_OUT_OF_POOL_MEMORY: return "ERROR : out of pool memory";
			case VK_ERROR_INVALID_EXTERNAL_HANDLE: return "ERROR : invalid external handle";
			case VK_ERROR_FRAGMENTATION: return "ERROR : fragmentation";
			case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS: return "ERROR : invalid opaque capture address";
			case VK_ERROR_SURFACE_LOST_KHR: return "ERROR : surface lost";
			case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR: return "ERROR : native window in use";
			case VK_SUBOPTIMAL_KHR: return "ERROR : suboptimal";
			case VK_ERROR_OUT_OF_DATE_KHR: return "ERROR : out of date";
			case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR: return "ERROR : incompatible display";
			case VK_ERROR_VALIDATION_FAILED_EXT: return "ERROR : validation failed";
			case VK_ERROR_INVALID_SHADER_NV: return "ERROR : invalid shader";
			case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT: return "ERROR : invalid DRM format modifier layout";
			case VK_ERROR_NOT_PERMITTED_EXT: return "ERROR : not permitted";
			case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT: return "ERROR : full screen exclusive mode lost";
			case VK_THREAD_IDLE_KHR: return "thread idle";
			case VK_THREAD_DONE_KHR: return "thread done";
			case VK_OPERATION_DEFERRED_KHR: return "operation deferred";
			case VK_OPERATION_NOT_DEFERRED_KHR: return "operation not deferred";
			case VK_PIPELINE_COMPILE_REQUIRED_EXT: return "pipeline compile required";
		}
		return "unknown result";
	}
}
