cmake_minimum_required(VERSION 3.25.1)

set(PROJECT_NAME DemonEngine)
project(${PROJECT_NAME})

add_subdirectory(DemonEngineCore)
add_subdirectory(DemonEngineEditor)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT DemonEngineEditor)

