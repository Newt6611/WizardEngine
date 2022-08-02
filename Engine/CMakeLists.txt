cmake_minimum_required(VERSION 3.10)
project(Engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(INCLUDE_DIRS 
    ${CMAKE_CURRENT_SOURCE_DIR}/src 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Core 
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/spdlog/include)


set(ALL_FILES 
    # Core
    src/Core/Application.cpp
    src/Core/Log.cpp
)

add_library(Engine ${ALL_FILES})


target_include_directories(Engine PUBLIC ${INCLUDE_DIRS})
target_precompile_headers(Engine PRIVATE src/wzpch.h)