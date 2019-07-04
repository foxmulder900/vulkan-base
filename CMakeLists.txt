cmake_minimum_required(VERSION 3.14)
project(vulkan_base)


set(CMAKE_CXX_STANDARD 17)

# Build GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(libs/glfw)

# Build GLM
add_subdirectory(libs/glm)

# Find pre-installed Vulkan binary
find_package(vulkan REQUIRED)

# Build and link app
add_executable(vulkan_base main.cpp)
target_link_libraries(vulkan_base glfw glm Vulkan::Vulkan)
