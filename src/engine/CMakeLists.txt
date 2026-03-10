set(ENGINE_SOURCES
    src/camera.cpp
    src/engine.cpp
)

add_library(engine STATIC ${ENGINE_SOURCES})

target_include_directories(engine PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(engine PUBLIC
    graphics
    glfw
    util
)

target_compile_definitions(engine PRIVATE
    RESOURCE_ROOT_PATH="${CMAKE_SOURCE_DIR}/src/graphics/assets"
)