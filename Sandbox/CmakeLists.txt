cmake_minimum_required(VERSION 3.10)

project(Sandbox)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(Sandbox src/Sandbox.cpp)

target_include_directories(Sandbox PRIVATE ${CMAKE_SOURCE_DIR}/Engine/src)

target_link_libraries(Sandbox Engine)