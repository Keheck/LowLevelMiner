set(SHADER_SOURCES
    fragment/default.shader
    vertex/default.shader
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
    set(SHADER_HEADERS ${SHADER_HEADERS} ${CMAKE_CURRENT_BINARY_DIR}/${SHADER_FILE}.h)
endforeach()

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/vertex)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/fragment)

add_executable(shdr2hdr ${CMAKE_CURRENT_SOURCE_DIR}/embed_shader.cpp)

add_custom_target(shaders DEPENDS ${SHADER_HEADERS})