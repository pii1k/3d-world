project(game_impl)

set(BAT_GAME_SOURCES
    src/bat_game.cpp
    src/bat_input.cpp
    src/bat_logic.cpp
    src/bat_scene_builder.cpp
    src/bat_camera_rig.cpp
)

add_library(${PROJECT_NAME} STATIC ${BAT_GAME_SOURCES})

target_include_directories(${PROJECT_NAME}
PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(${PROJECT_NAME}
PUBLIC
    domain
    engine
)

target_link_libraries(${PROJECT_NAME}
PRIVATE
    graphics
    glfw
)
