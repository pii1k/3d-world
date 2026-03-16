project(engine)

set(ENGINE_SOURCES
    src/camera.cpp
    src/engine.cpp
)

add_library(${PROJECT_NAME} STATIC ${ENGINE_SOURCES})

target_include_directories(${PROJECT_NAME}
PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(${PROJECT_NAME}
PUBLIC
    graphics
    glfw
    util
)

target_compile_definitions(${PROJECT_NAME}
PRIVATE
    RESOURCE_ROOT_PATH="${CMAKE_SOURCE_DIR}/src/graphics/assets"
)