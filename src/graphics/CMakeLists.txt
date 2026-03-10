project(graphics)

set(GRAPHICS_SOURCES
    src/mesh.cpp
    src/renderer.cpp
    src/shader.cpp
)

add_library(graphics STATIC ${GRAPHICS_SOURCES})

target_include_directories(graphics PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(graphics PUBLIC
    glad
    glm
    domain
    util
)