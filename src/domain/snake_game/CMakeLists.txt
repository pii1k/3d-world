project(game_impl)

set(SNAKE_GAME_SOURCES
    src/snake_game.cpp
    src/snake_camera_rig.cpp
    src/snake_input.cpp
    src/snake_logic.cpp
    src/snake_scene_builder.cpp
)

add_library(${PROJECT_NAME} STATIC ${SNAKE_GAME_SOURCES})

target_include_directories(${PROJECT_NAME}
PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(${PROJECT_NAME}
PUBLIC
    domain
)

target_link_libraries(${PROJECT_NAME}
PRIVATE
    graphics
)