cmake_minimum_required(VERSION 3.10.0)
project(LowLevelMiner VERSION 0.1.0 LANGUAGES C CXX)

find_library(GLFW_LIB glfw)

add_subdirectory(glad)
add_subdirectory(shader)

add_executable(LowLevelMiner main.cpp
    ${SHADER_HEADERS}
)

target_include_directories(LowLevelMiner PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/shader)

add_dependencies(LowLevelMiner shaders)

target_link_libraries(LowLevelMiner PUBLIC "${GLFW_LIB}")
target_link_libraries(LowLevelMiner PUBLIC glad)