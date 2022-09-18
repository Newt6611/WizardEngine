cmake_minimum_required(VERSION 3.10)

project(Sandbox)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# platforms
if (WIN32)
    add_definitions(-DWZ_WINDOWS)
elseif(APPLE)
    add_definitions(-DWZ_APPLE)
elseif(UNIX AND NOT APPLE)
    add_definitions(-DWZ_LINUX)
endif()


add_executable(Sandbox src/Sandbox.cpp)
target_include_directories(Sandbox PRIVATE ${CMAKE_SOURCE_DIR}/Engine/src)

target_link_libraries(Sandbox Engine)