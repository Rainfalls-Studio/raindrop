#pragma once

#include "Raindrop/Core/Utils/Flags.hpp"

namespace Raindrop::Graphics::Backend{
    enum class PipelineStageBits{
        NONE = 0,
        TOP_OF_PIPE = 1 << 0,
        DRAW_INDIRECT = 1 << 1,
        VERTEX_INPUT = 1 << 2,
        VERTEX_SHADER = 1 << 3,
        TESSELLATION_CONTROL_SHADER = 1 << 4,
        TESSELLATION_EVALUATION_SHADER = 1 << 5,
        GEOMETRY_SHADER = 1 << 6,
        FRAGMENT_SHADER = 1 << 7,
        EARLY_FRAGMENT_TESTS = 1 << 8,
        LATE_FRAGMENT_TESTS = 1 << 9,
        COLOR_ATTACHMENT_OUTPUT = 1 << 10,
        COMPUTE_SHADER = 1 << 11,
        TRANSFER = 1 << 12,
        BOTTOM_OF_PIPE = 1 << 13,
        HOST = 1 << 14,
        ALL_GRAPHICS = 1 << 15,
        ALL_COMMANDS = 1 << 16,
        TRANSFORM_FEEDBACK = 1 << 17,
        CONDITIONAL_RENDERING = 1 << 18,
        ACCELERATION_STRUCTURE_BUILD = 1 << 19,
        RAY_TRACING_SHADER = 1 << 20,
        FRAGMENT_DENSITY_PROCESS = 1 << 21,
        FRAGMENT_SHADING_RATE_ATTACHMENT = 1 << 22,
        COMMAND_PREPROCESS = 1 << 23,
        TASK_SHADER = 1 << 24,
        MESH_SHADER = 1 << 25,
        __SIZE__
    };

    RAINDROP_FLAG(PipelineStage, PipelineStageBits)
}