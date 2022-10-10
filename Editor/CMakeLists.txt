cmake_minimum_required(VERSION 3.10)

project(Editor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


if (WIN32)
    add_definitions(-DWZ_WINDOWS)
elseif(APPLE)
    add_definitions(-DWZ_APPLE)
elseif(UNIX AND NOT APPLE)
    add_definitions(-DWZ_LINUX)
endif()

set(SOURCE_DIRS 
    src/Panels/ComponentPanel.cpp
    src/EditorLayer.cpp
    src/ImGuiD3D12.cpp
)

add_executable(Editor src/WZEditor.cpp ${SOURCE_DIRS})
target_include_directories(Editor PRIVATE ${CMAKE_SOURCE_DIR}/Engine/src)
target_include_directories(Editor PRIVATE ${CMAKE_SOURCE_DIR}/Engine)

target_link_libraries(Editor Engine)

if (WIN32)
    set_target_properties(Editor PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/assets")
endif()