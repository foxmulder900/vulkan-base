
#include <vulkan/vulkan.h>
#include <vector>
#include <stdexcept>

#include <swapchain.cpp>

class FrameBuffer{

    std::vector<VkFramebuffer> frameBuffers;

public:
    void init(VkDevice* device, SwapChain* swapChain, VkRenderPass* renderPass){
        int imageCount = swapChain->getSize();
        frameBuffers.resize(imageCount);
        for (int i = 0; i < imageCount; i++) {
            VkImageView attachments[] = {
                    *swapChain->getImageView(i)
            };

            VkExtent2D extent = swapChain->getExtent();

            VkFramebufferCreateInfo framebufferInfo = {};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = *renderPass;
            framebufferInfo.attachmentCount = 1;
            framebufferInfo.pAttachments = attachments;
            framebufferInfo.width = extent.width;
            framebufferInfo.height = extent.height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(*device, &framebufferInfo, nullptr, &frameBuffers[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to create framebuffer!");
            }
        }
    }

    VkImageView* getBuffer(int index){
        return &frameBuffers[index];
    }

    void cleanup(VkDevice* device){
        for (auto framebuffer : frameBuffers) {
            vkDestroyFramebuffer(*device, framebuffer, nullptr);
        }
    }
};