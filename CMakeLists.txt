cmake_minimum_required(VERSION 3.14)
project(learning_vulkan)


set(CMAKE_CXX_STANDARD 17)
set(VK_ICD_FILENAMES ../vulkansdk-macos-1.1.108.0/macOS/etc/vulkan/icd.d/MoltenVK_icd.json)
set(VK_LAYER_PATH ../vulkansdk-macos-1.1.108.0/macOS/etc/vulkan/explicit_layer.d)

include_directories('/usr/local/include')
include_directories('../vulkansdk-macos-1.1.108.0/macOS/include')

link_directories('/usr/local/lib')
link_directories('../vulkansdk-macos-1.1.108.0/macOS/lib')

find_package(glfw3 3.4 REQUIRED)
find_package(vulkan REQUIRED)
find_package(glm REQUIRED)

add_executable(learning_vulkan main.cpp)

target_link_libraries(learning_vulkan glfw glm Vulkan::Vulkan)
