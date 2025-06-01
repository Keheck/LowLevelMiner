set(FRAGMENT_SHADERS
    mix.shader
    lit.shader
    light.shader
    unlit.shader
    depth_vis.shader
    outline.shader
    framebuffer.shader
    skybox.shader
)

set(VERTEX_SHADERS
    default.shader
    framebuffer.shader
    light.shader
    skybox.shader
)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/fragment)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/vertex)

list(TRANSFORM FRAGMENT_SHADERS PREPEND "fragment/")
list(TRANSFORM VERTEX_SHADERS PREPEND "vertex/")

file(COPY ${FRAGMENT_SHADERS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/fragment)
file(COPY ${VERTEX_SHADERS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/vertex)