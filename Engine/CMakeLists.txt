cmake_minimum_required(VERSION 3.10)
project(Engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# build types
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DWZ_DEBUG)
elseif(CMAKE_BUILD_TYPE EQUAL "Release")
    add_definitions(-DWZ_RELEASE)
elseif(CMAKE_BUILD_TYPE EQUAL "Dist")
    add_definitions(-DWZ_DIST)
endif()

# platforms
if (WIN32)
    add_definitions(-DWZ_WINDOWS)
elseif(APPLE)
    add_definitions(-DWZ_APPLE)
elseif(UNIX AND NOT APPLE)
    add_definitions(-DWZ_LINUX)
endif()

#source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR})

find_package(Vulkan REQUIRED)
# All Files
set(ALL_FILES 
    src/Core.h
    src/EntryPoint.h
    src/Wizard.h
    src/wzpch.h

    # Core
    src/Core/Application.h
    src/Core/Application.cpp
    src/Core/Log.h
    src/Core/Log.cpp
    src/Core/Window.h
    src/Core/Window.cpp

    # Renderer
    src/Renderer/Shader.h
    src/Renderer/Shader.cpp
    src/Renderer/Renderer.h
    src/Renderer/Renderer.cpp
    src/Renderer/VulkanRenderer.h
    src/Renderer/VulkanRenderer.cpp
    src/Renderer/VulkanShader.h
    src/Renderer/VulkanShader.cpp
)

# Include Directories
set(INCLUDE_DIRS 
    ${CMAKE_CURRENT_SOURCE_DIR}/src 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Core 
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty 
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/spdlog/include 
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glfw/include
    Vulkan::Vulkan
)

# Libs
set(LINK_LIBS 
    glfw 
    Vulkan::Vulkan
)

# glfw: prevent glfw to build test or docmentation
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
#


add_library(Engine ${ALL_FILES})
add_subdirectory(thirdparty/DiligentCore)
add_subdirectory(thirdparty/glfw)

target_precompile_headers(Engine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/wzpch.h)
target_include_directories(Engine PUBLIC ${INCLUDE_DIRS})
target_link_libraries(Engine PUBLIC ${LINK_LIBS})


