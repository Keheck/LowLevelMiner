cmake_minimum_required(VERSION 3.10.0)
project(LowLevelMiner VERSION 0.1.0 LANGUAGES C CXX)

find_library(GLFW_LIB glfw)

add_subdirectory(glad)
add_subdirectory(shader)

add_executable(LowLevelMiner
    src/main.cpp
    src/shader.cpp
)

target_include_directories(LowLevelMiner PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/shader)
target_include_directories(LowLevelMiner PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_dependencies(LowLevelMiner shaders)

target_link_libraries(LowLevelMiner PUBLIC "${GLFW_LIB}")
target_link_libraries(LowLevelMiner PUBLIC glad)