#pragma once

#include <string>
#include <vulkan/vulkan.hpp>

namespace Raindrop::Graphics::Common{
    struct ResourceDescription{
        /**
         * @brief The name of the resource, used for debug and information purposes.
         * 
         */
        std::string name;

        /**
         * @brief The type of the resource
         * 
         */
        enum class Type{
            ATTACHMENT,
            BUFFER
        } type = Type::ATTACHMENT;

        struct Attachment{
            vk::Format format;
            vk::Extent2D extent;
            vk::ImageLayout layout;
            vk::ImageUsageFlags usage;
        };

        struct Buffer{
            vk::DeviceSize size;
            vk::BufferUsageFlags usage;
        };

        union {
            Attachment attachment = {};
            Buffer buffer;
        };
    };
}