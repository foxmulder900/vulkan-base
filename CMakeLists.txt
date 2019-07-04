cmake_minimum_required(VERSION 3.14)
project(vulkan_base)


set(CMAKE_CXX_STANDARD 17)

if(APPLE)
    message('APPLE system detected...')
    set(VK_ICD_FILENAMES ../vulkansdk-macos-1.1.108.0/macOS/etc/vulkan/icd.d/MoltenVK_icd.json)
    set(VK_LAYER_PATH ../vulkansdk-macos-1.1.108.0/macOS/etc/vulkan/explicit_layer.d)

    include_directories('/usr/local/include')
    include_directories('../vulkansdk-macos-1.1.108.0/macOS/include')

    link_directories('/usr/local/lib')
    link_directories('../vulkansdk-macos-1.1.108.0/macOS/lib')

    # TODO: see if we can just build these on OSX too
    find_package(glfw3 REQUIRED)
    find_package(vulkan REQUIRED)
    find_package(glm REQUIRED)
endif(APPLE)

if(WIN32)
    message("WIN32 system detected...")

    # Build GLFW
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    add_subdirectory(libs/glfw)

    # Build GLM
    add_subdirectory(libs/glm)

    # Find installed Vulkan binary
    find_package(vulkan REQUIRED)
endif(WIN32)

add_executable(vulkan_base main.cpp)

target_link_libraries(vulkan_base glfw glm Vulkan::Vulkan)
