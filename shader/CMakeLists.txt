set(SHADER_SOURCES
    fragment/default.shader
    vertex/default.shader
    fragment/mix.shader
    fragment/lit.shader
    fragment/light.shader
    vertex/light.shader
)

set(SHADER_HEADERS "")

function(register_shader SHADER)
    set(SHADER_HEADER "${CMAKE_CURRENT_BINARY_DIR}/${SHADER}.h")
    add_custom_command(OUTPUT "${SHADER_HEADER}"
        COMMAND shdr2hdr "${CMAKE_CURRENT_SOURCE_DIR}/${SHADER}" "${SHADER_HEADER}" "$<MAKE_C_IDENTIFIER:${SHADER}>"
        DEPENDS "${SHADER}" shdr2hdr
        VERBATIM)
endfunction()

foreach(SHADER_FILE ${SHADER_SOURCES})
    register_shader("${SHADER_FILE}")
    list(APPEND SHADER_HEADERS ${CMAKE_CURRENT_BINARY_DIR}/${SHADER_FILE}.h)
endforeach()

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/vertex)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/fragment)

add_executable(shdr2hdr ${CMAKE_CURRENT_SOURCE_DIR}/embed_shader.cpp)

list(TRANSFORM SHADER_SOURCES PREPEND "#include \"" OUTPUT_VARIABLE SHADER_INCLUDES)
list(TRANSFORM SHADER_INCLUDES APPEND ".h\"\n")
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/shaders.h "#pragma once\n")
file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/shaders.h ${SHADER_INCLUDES})

add_custom_target(shaders DEPENDS ${SHADER_HEADERS})