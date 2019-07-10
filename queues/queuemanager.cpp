#include "QueueFamilyIndices.cpp"
#include "syncobjects.cpp"

const int MAX_FRAMES_IN_FLIGHT = 2;

class QueueManager{
    VkQueue graphicsQueue;
    VkQueue presentQueue;

    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;

public:
    void init(VkDevice& device, QueueFamilyIndices queueFamilyIndices){
        std::cout << "Initializing queue manager..." << std::endl;
        vkGetDeviceQueue(device, queueFamilyIndices.graphicsFamily.value(), 0, &graphicsQueue);
        vkGetDeviceQueue(device, queueFamilyIndices.presentFamily.value(), 0, &presentQueue);
        createSyncObjects(device);
    }

    void submitToGraphicsQueue(VkSubmitInfo submitInfo, size_t currentFrame, std::vector<VkSemaphore>& imageAvailableSemaphores){
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.pWaitDstStageMask = waitStages;

        VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;

        VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
            throw std::runtime_error("failed to submit draw command buffer!");
        }
    }

    void submitToPresentQueue(VkPresentInfoKHR presentInfo, size_t currentFrame){
        VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;
        vkQueuePresentKHR(presentQueue, &presentInfo);
    }

    void waitForFences(VkDevice& device, size_t currentFrame){
        vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max());
        vkResetFences(device, 1, &inFlightFences[currentFrame]);
    }

    void cleanup(VkDevice& device){
        destroySemaphores(device, renderFinishedSemaphores);
        destroyFences(device, inFlightFences);
    }

private:
    void createSyncObjects(VkDevice& device){
        renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
        createSemaphores(device, renderFinishedSemaphores);
        createFences(device, inFlightFences);
    }
};