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
endif()

# Assets
set(ASSETS 
    assets
    src
)

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
)

# Include Directories
set(INCLUDE_DIRS 
    src
    src/Core 
    thirdparty/DiligentCore
    thirdparty/spdlog/include 
    thirdparty/glfw/include
)

# Libs
set(LINK_LIBS 
    Diligent-Common 
    glfw
)

# glfw: prevent glfw to build test or docmentation
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
#

add_library(Engine ${ALL_FILES})
add_subdirectory(thirdparty/DiligentCore)
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
