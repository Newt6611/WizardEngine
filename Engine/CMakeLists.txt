cmake_minimum_required(VERSION 3.10)
project(Engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${CMAKE_SOURCE_DIR}/Engine/src)
include_directories(${CMAKE_SOURCE_DIR}/Engine/src/Wizard)

set(ALL_FILES 
    src/EntryPoint.cpp 

    # Wizard
    src/Wizard/Application.cpp
)

add_library(Engine ${ALL_FILES})