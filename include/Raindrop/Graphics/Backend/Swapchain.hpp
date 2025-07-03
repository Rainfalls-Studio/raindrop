#pragma once

#include <cstdint>
#include <vector>
#include <memory>
#include "Extent.hpp"
#include "Format.hpp"
#include "Image.hpp"
#include "Queue.hpp"
#include "Surface.hpp"
#include "Semaphore.hpp"
#include "Fence.hpp"
#include "API.hpp"

namespace Raindrop::Graphics::Backend{
    class Swapchain{
        public:
            enum class PresentMode{
                // Dosen't wait and immediatly present (causes tearning but minimal delay)
                IMMEDIATE = 0,

                // Wait for the image to be available (v-sync)
                MAILBOX = 1,

                // Wait for the last images to be presented (blocking v-sync)
                FIFO = 2,
            };

            struct Description{
                std::shared_ptr<Surface> surface;
                uint32_t imageCount;
                Format format;
                PresentMode presentMode;
            };

            virtual ~Swapchain() = default;
            
            /**
            * @brief Get the number of images in the swapchain.
             * 
             * @return uint32_t 
             */
            virtual uint32_t getImageCount() const = 0;
            
            /**
             * @brief Get the format of the images in the swapchain.
             * 
             * @return Format 
             */
            virtual Format getFormat() const = 0;

            /**
             * @brief Get the extent of the images in the swapchain.
             * 
             * @return Extent2D 
             */
            virtual Extent2D getExtent() const = 0;

            /**
             * @brief Get the images of the swapchain
             * 
             * @return const std::vector<std::shared_ptr<Image>>& 
             */
            virtual const std::vector<std::shared_ptr<Image>>& getImages() const = 0;

            /**
            * @brief Rebuild the swapchain, should be used after window resize
             * 
             */
            virtual void rebuild() = 0;
            
            /**
             * @brief Get the next image in the swapchain
             * 
             * @param imageIndex The index of the next available image
             * @param signalSemaphore The semaphore to signal once the image is available
             * @param signalFence The fence to signal once the image is available
             * @param timeout The time to waite (in nanoseconds) 
             * @return uint32_t 
             */
            virtual uint32_t acquireNextImage(
                uint32_t& imageIndex,
                std::shared_ptr<Semaphore> signalSemaphore = nullptr,
                std::shared_ptr<Fence> signalFence = nullptr,
                uint32_t timeout = UINT32_MAX
            ) = 0;

            /**
             * @brief Present the swapchain image.
             * 
             * @param queue The queue to use
             * @param imageIndex the image to present
             * @param waitSemaphores Waits until the semaphore is signaled
             */
            virtual void present(
                std::shared_ptr<Queue> queue,
                uint32_t imageIndex,
                const std::vector<std::shared_ptr<Semaphore>>& waitSemaphores = {}
            ) = 0;
            
            /**
             * @brief Set the present mode of the swapchain
             * 
             * @param mode 
             */
            virtual void setPresentMode(PresentMode mode) = 0;

            virtual void* getHandle() const noexcept = 0;
            virtual API getAPI() const noexcept = 0;
    };
}