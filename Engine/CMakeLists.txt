cmake_minimum_required(VERSION 3.10)
project(Engine)

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


# Assets
set(ASSETS_SHADERS
    assets/shaders/cube.ps
    assets/shaders/cube.vs
)

set(ASSETS_TEXTURES
    assets/textures/DGLogo.png
)

set(ASSETS ${ASSETS_SHADERS} ${ASSETS_TEXTURES})
#


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
    src/Renderer/RenderPipelineState.cpp
    src/Renderer/VertexBuffer.cpp
    src/Renderer/IndexBuffer.cpp
    src/Renderer/OrthographicCamera.cpp
)

# Include Directories
set(INCLUDE_DIRS
    src
    src/Core 
    thirdparty/DiligentCore
    thirdparty/DiligentTools
    thirdparty/spdlog/include 
    thirdparty/glfw/include
    thirdparty/glm
)

# Libs
set(LINK_LIBS 
    Diligent-Common 
    Diligent-GraphicsTools 
    Diligent-TextureLoader 
    glfw 
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

if(PLATFORM_WIN32)
    #set_target_properties(Engine PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/assets")
    target_sources(Engine PRIVATE ${ASSETS})
    source_group("assets/shaders" FILES ${ASSETS_SHADERS})
    source_group("assets/textures" FILES ${ASSETS_TEXTURES})
endif()