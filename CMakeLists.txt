cmake_minimum_required(VERSION 3.15)

project(vulkan_block_game)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

find_package(glfw3 REQUIRED)

find_package(Vulkan REQUIRED)

add_executable(vulkan_block_game main.cpp)

add_dependencies(vulkan_block_game glfw Vulkan::Vulkan)

target_link_libraries(vulkan_block_game glfw Vulkan::Vulkan)
