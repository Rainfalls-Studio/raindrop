#pragma once

#include "../PipelineStage.hpp"
#include "translation.hpp"
#include "vulkan/vulkan_core.h"
#include <stdexcept>
#include <vulkan/vulkan.h>

namespace Raindrop::Graphics::Backend::Vulkan{
    template<>
    VkPipelineStageFlagBits toVulkan(PipelineStageBits&& bit){
        using enum PipelineStageBits;
        switch (bit){
            case NONE: return VK_PIPELINE_STAGE_NONE;
            case TOP_OF_PIPE: return VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            case DRAW_INDIRECT: return VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT;
            case VERTEX_INPUT: return VK_PIPELINE_STAGE_VERTEX_INPUT_BIT;
            case VERTEX_SHADER: return VK_PIPELINE_STAGE_VERTEX_SHADER_BIT;
            case TESSELLATION_CONTROL_SHADER: return VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT;
            case TESSELLATION_EVALUATION_SHADER: return VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT;
            case GEOMETRY_SHADER: return VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT;
            case FRAGMENT_SHADER: return VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            case EARLY_FRAGMENT_TESTS: return VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
            case LATE_FRAGMENT_TESTS: return VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
            case COLOR_ATTACHMENT_OUTPUT: return VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            case COMPUTE_SHADER: return VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
            case TRANSFER: return VK_PIPELINE_STAGE_TRANSFER_BIT;
            case BOTTOM_OF_PIPE: return VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
            case HOST: return VK_PIPELINE_STAGE_HOST_BIT;
            case ALL_GRAPHICS: return VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT;
            case ALL_COMMANDS: return VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
            case TRANSFORM_FEEDBACK: return VK_PIPELINE_STAGE_TRANSFORM_FEEDBACK_BIT_EXT;
            case CONDITIONAL_RENDERING: return VK_PIPELINE_STAGE_CONDITIONAL_RENDERING_BIT_EXT;
            case ACCELERATION_STRUCTURE_BUILD: return VK_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_KHR;
            case RAY_TRACING_SHADER: return VK_PIPELINE_STAGE_RAY_TRACING_SHADER_BIT_KHR;
            case FRAGMENT_DENSITY_PROCESS: return VK_PIPELINE_STAGE_FRAGMENT_DENSITY_PROCESS_BIT_EXT;
            case FRAGMENT_SHADING_RATE_ATTACHMENT: return VK_PIPELINE_STAGE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR;
            case COMMAND_PREPROCESS: return VK_PIPELINE_STAGE_COMMAND_PREPROCESS_BIT_EXT;
            case TASK_SHADER: return VK_PIPELINE_STAGE_TASK_SHADER_BIT_EXT;
            case MESH_SHADER: return VK_PIPELINE_STAGE_MESH_SHADER_BIT_EXT;
            default: break;
        }
        throw std::runtime_error("Undefined pipeline stage");
    }

    RAINDROP_TO_VK_FLAG(VkPipelineStageFlags, VkPipelineStageFlagBits, PipelineStage)
}