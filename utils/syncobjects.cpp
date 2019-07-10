#ifndef SYNC_OBJECTS
#define SYNC_OBJECTS

void createSemaphore(VkDevice& device, VkSemaphore& semaphore){
    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &semaphore) != VK_SUCCESS) {
        throw std::runtime_error("failed to create synchronization object for a frame!");
    }
}

void createSemaphores(VkDevice& device, std::vector<VkSemaphore>& semaphores){
    for (size_t i = 0; i < semaphores.size(); i++) {
        createSemaphore(device, semaphores[i]);
    }
}

void destroySemaphores(VkDevice& device, std::vector<VkSemaphore>& semaphores){
    for (size_t i = 0; i < semaphores.size(); i++) {
        vkDestroySemaphore(device, semaphores[i], nullptr);
    }
}

void createFence(VkDevice& device, VkFence& fence){
    VkFenceCreateInfo fenceInfo = {};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    if (vkCreateFence(device, &fenceInfo, nullptr, &fence) != VK_SUCCESS) {
        throw std::runtime_error("failed to create synchronization object for a frame!");
    }
}

void createFences(VkDevice& device, std::vector<VkFence>& fences){
    for (size_t i = 0; i < fences.size(); i++) {
        createFence(device, fences[i]);
    }
}

void destroyFences(VkDevice& device, std::vector<VkFence >& fences){
    for (size_t i = 0; i < fences.size(); i++) {
        vkDestroyFence(device, fences[i], nullptr);
    }
}

#endif