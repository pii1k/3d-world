add_library(graphics STATIC
    src/renderer.cpp
    src/camera.cpp
)

target_include_directories(graphics
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(graphics
    PUBLIC
        OpenGL::GL
        glfw
)

target_compile_definitions(graphics
PUBLIC
    SHADER_ASSET_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets"
)