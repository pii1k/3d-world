add_library(app STATIC
    src/engine.cpp
    src/input_controller.cpp
    src/prefabs.cpp
    src/ui_layer.cpp
)

target_include_directories(app
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(app
PUBLIC
    ecs
    graphics
    imgui
)