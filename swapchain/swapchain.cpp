#include <iostream>
#include <algorithm>
#include "QueueFamilyIndices.cpp"

#ifndef SWAPCHAIN
#define SWAPCHAIN
    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    class SwapChain {
        VkSurfaceFormatKHR surfaceFormat;
        VkSwapchainKHR swapchain;
        std::vector<VkImage> images;
        VkExtent2D extent;
        std::vector<VkImageView> imageViews;


    public:
        void init(VkPhysicalDevice& physicalDevice, VkDevice& device, VkSurfaceKHR& surface, uint32_t desired_width, uint32_t desired_height, QueueFamilyIndices queueFamilyIndices) {
            std::cout << "Initializing swap chain..." << std::endl;
            SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice, surface);

            chooseSwapSurfaceFormat(swapChainSupport.formats);
            VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
            extent = chooseSwapExtent(swapChainSupport.capabilities, desired_width, desired_height);

            uint32_t imageCount = getImageCount(swapChainSupport);

            createSwapChain(device, surface, queueFamilyIndices, swapChainSupport, presentMode, imageCount);

            vkGetSwapchainImagesKHR(device, swapchain, &imageCount, nullptr);
            images.resize(imageCount);
            vkGetSwapchainImagesKHR(device, swapchain, &imageCount, images.data());

            createImageViews(device, surfaceFormat.format);
        }

        VkExtent2D& getExtent(){
            return extent;
        }

        VkFormat& getImageFormat(){
            return surfaceFormat.format;
        }

        size_t getSize(){
            return images.size();
        }

        VkImageView& getImageView(int index){
            return imageViews[index];
        }

        VkSwapchainKHR& getSwapChain(){
            return swapchain;
        }

        static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice& availableDevice, VkSurfaceKHR& surface) {
            SwapChainSupportDetails details;

            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(availableDevice, surface, &details.capabilities);

            uint32_t formatCount;
            vkGetPhysicalDeviceSurfaceFormatsKHR(availableDevice, surface, &formatCount, nullptr);
            if (formatCount != 0) {
                details.formats.resize(formatCount);
                vkGetPhysicalDeviceSurfaceFormatsKHR(availableDevice, surface, &formatCount, details.formats.data());
            }

            uint32_t presentModeCount;
            vkGetPhysicalDeviceSurfacePresentModesKHR(availableDevice, surface, &presentModeCount, nullptr);
            if (presentModeCount != 0) {
                details.presentModes.resize(presentModeCount);
                vkGetPhysicalDeviceSurfacePresentModesKHR(availableDevice, surface, &presentModeCount, details.presentModes.data());
            }

            return details;
        }

        uint32_t acquireNewImage(VkDevice& device, VkSemaphore signalSemaphore){
            uint32_t imageIndex;
            vkAcquireNextImageKHR(device, swapchain, std::numeric_limits<uint64_t>::max(), signalSemaphore, VK_NULL_HANDLE, &imageIndex);
            return imageIndex;
        }

        void cleanup(VkDevice& device){
            for (auto imageView : imageViews) {
                vkDestroyImageView(device, imageView, nullptr);
            }
            vkDestroySwapchainKHR(device, swapchain, nullptr);
        }

    private:

        void createSwapChain(VkDevice& device, VkSurfaceKHR& surface, QueueFamilyIndices queueFamilyIndices,
                             SwapChainSupportDetails &swapChainSupport,
                             VkPresentModeKHR &presentMode,
                             uint32_t imageCount)  {
            VkSwapchainCreateInfoKHR createInfo = {};
            createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
            createInfo.surface = surface;
            createInfo.minImageCount = imageCount;
            createInfo.imageFormat = surfaceFormat.format;
            createInfo.imageColorSpace = surfaceFormat.colorSpace;
            createInfo.imageExtent = extent;
            createInfo.imageArrayLayers = 1;
            createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

            if (queueFamilyIndices.graphicsFamily != queueFamilyIndices.presentFamily) {
                uint32_t indices[] = {queueFamilyIndices.graphicsFamily.value(), queueFamilyIndices.presentFamily.value()};
                createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
                createInfo.queueFamilyIndexCount = 2;
                createInfo.pQueueFamilyIndices = indices;
                std::cout << "Using concurrent queue sharing mode." << std::endl;
            } else {
                createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
                createInfo.queueFamilyIndexCount = 0; // Optional
                createInfo.pQueueFamilyIndices = nullptr; // Optional
                std::cout << "Using exclusive queue sharing mode." << std::endl;
            }

            createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
            createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
            createInfo.presentMode = presentMode;
            createInfo.clipped = VK_TRUE;
            createInfo.oldSwapchain = VK_NULL_HANDLE;

            if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapchain) != VK_SUCCESS) {
                throw std::runtime_error("failed to create swap chain!");
            }
        }

        uint32_t getImageCount(const SwapChainSupportDetails &swapChainSupport) const {
            uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
            if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
                imageCount = swapChainSupport.capabilities.maxImageCount;
            }
            return imageCount;
        }

        void chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
            for (const auto& availableFormat : availableFormats) {
                if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                    surfaceFormat = availableFormat;
                    return;
                }
            }
            surfaceFormat = availableFormats[0];
        }

        static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
            VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

            for (const auto& availablePresentMode : availablePresentModes) {
                if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                    bestMode = availablePresentMode;
                } else if (availablePresentMode == VK_PRESENT_MODE_FIFO_RELAXED_KHR) {
                    bestMode = availablePresentMode;
                } else if (availablePresentMode == VK_PRESENT_MODE_FIFO_KHR) {
                    bestMode = availablePresentMode;
                } else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
                    bestMode = availablePresentMode;
                }
            }

            return bestMode;
        }

        static VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, uint32_t width, uint32_t height) {
            if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
                return capabilities.currentExtent;
            } else {
                VkExtent2D actualExtent = {width, height};
                actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
                actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
                return actualExtent;
            }
        }

        void createImageViews(VkDevice& device, VkFormat imageFormat) {
            imageViews.resize(images.size());

            for (size_t i = 0; i < images.size(); i++) {
                VkImageViewCreateInfo createInfo = {};
                createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
                createInfo.image = images[i];

                createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
                createInfo.format = imageFormat;

                createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
                createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
                createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
                createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

                createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                createInfo.subresourceRange.baseMipLevel = 0;
                createInfo.subresourceRange.levelCount = 1;
                createInfo.subresourceRange.baseArrayLayer = 0;
                createInfo.subresourceRange.layerCount = 1;

                if (vkCreateImageView(device, &createInfo, nullptr, &imageViews[i]) != VK_SUCCESS) {
                    throw std::runtime_error("failed to create image views!");
                }
            }
        }


    };

#endif