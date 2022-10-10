# notes: DiligentTools/ThirdParty/imgui is not a submodule anymore(Diligent)
# it's clone by myself. 

cmake_minimum_required(VERSION 3.10)
# project(Engine LANGUAGES CXX OBJC OBJCXX)
project(Engine LANGUAGES)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

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
    find_package(Threads)
endif()
if(PLATFORM_MACOS)
    list(APPEND SOURCES src/Renderer/SurfaceHelper.mm)
endif()

# Source Files
set(SOURCE_DIRS 
    # Core
    src/Core/Application.cpp
    src/Core/Log.cpp
    src/Core/Window.cpp
    src/Core/LayerStack.cpp

    # Renderer
    src/Renderer/Shader.cpp
    src/Renderer/Renderer.cpp
    src/Renderer/Renderer2D.cpp
    src/Renderer/RenderPipelineState.cpp
    src/Renderer/Buffer.cpp
    src/Renderer/Texture.cpp
    src/Renderer/OrthographicCamera.cpp
    src/Renderer/OrthographicCameraController.cpp
    src/Renderer/PerspectiveCamera.cpp
    

    # Scene
    src/Scene/Scene.cpp
    src/Scene/SceneSerializer.cpp

    # Platform
    src/Platform/WindowsInput.cpp

    # Helper
    # src/Renderer/SurfaceHelper.mm

    # thirdparty Diligent Imgui
    thirdparty/DiligentTools/ThirdParty/imgui/backends/imgui_impl_glfw.cpp
    thirdparty/DiligentTools/ThirdParty/imgui/backends/imgui_impl_dx12.cpp
)

# Include Directories
set(INCLUDE_DIRS
    src
    src/Core 
    thirdparty/DiligentCore
    thirdparty/DiligentTools 
    
    thirdparty 
    thirdparty/box2d/include
    thirdparty/spdlog/include 
    thirdparty/glfw/include
    thirdparty/glm
    thirdparty/entt/single_include
    thirdparty/yaml-cpp/include
)

# Libs
set(LINK_LIBS 
    Diligent-Common 
    Diligent-GraphicsTools 
    Diligent-Imgui 
    Diligent-TextureLoader 
    glfw
    box2d
    yaml-cpp::yaml-cpp
)

# glfw: prevent glfw to build test or docmentation
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
#

add_library(Engine ${SOURCE_DIRS})
add_subdirectory(thirdparty/DiligentCore)
add_subdirectory(thirdparty/DiligentTools)
add_subdirectory(thirdparty/glfw)
add_subdirectory(thirdparty/box2d)
add_subdirectory(thirdparty/yaml-cpp)

# Diligent
if(D3D11_SUPPORTED)
    target_link_libraries(Engine PRIVATE Diligent-GraphicsEngineD3D11-shared)
    add_definitions(-DWZ_D3D11)
endif()
if(D3D12_SUPPORTED)
    target_link_libraries(Engine PRIVATE Diligent-GraphicsEngineD3D12-shared)
    add_definitions(-DWZ_D3D12)
endif()
if(GL_SUPPORTED)
    target_link_libraries(Engine PRIVATE Diligent-GraphicsEngineOpenGL-shared)
    add_definitions(-DWZ_GL)
endif()
if(VULKAN_SUPPORTED)
    target_link_libraries(Engine PRIVATE Diligent-GraphicsEngineVk-shared)
    add_definitions(-DWZ_VULKAN)
endif()
if(METAL_SUPPORTED)
    target_link_libraries(Engine PRIVATE Diligent-GraphicsEngineMetal-shared)
    add_definitions(-DWZ_METAL)
endif()

if(PLATFORM_WIN32)
    copy_required_dlls(Engine)
endif()

target_compile_definitions(Engine PRIVATE ENGINE_DLL=1)
target_precompile_headers(Engine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/wzpch.h)
target_include_directories(Engine PUBLIC ${INCLUDE_DIRS})
target_link_libraries(Engine PUBLIC ${LINK_LIBS})