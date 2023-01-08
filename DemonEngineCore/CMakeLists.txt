cmake_minimum_required(VERSION 3.25.1)

set(ENGINE_PROJECT_NAME DemonEngineCore)

add_library(${ENGINE_PROJECT_NAME} STATIC
	src/DemonEngineCore/utils/test.cpp
	includes/DemonEngineCore/utils/test.hpp
)

target_include_directories(${ENGINE_PROJECT_NAME} PUBLIC includes)
target_compile_features(${ENGINE_PROJECT_NAME} PUBLIC cxx_std_17)


add_subdirectory(../external/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE glfw)