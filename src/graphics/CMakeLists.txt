project(graphics)

set(GRAPHICS_SOURCES
    src/mesh.cpp
    src/renderer.cpp
    src/shader.cpp
)

add_library(${PROJECT_NAME} STATIC ${GRAPHICS_SOURCES})

target_include_directories(${PROJECT_NAME}
PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(${PROJECT_NAME}
PUBLIC
    glad
    glm
    domain
    util
)