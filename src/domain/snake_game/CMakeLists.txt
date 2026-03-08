set(SNAKE_GAME_SOURCES
    src/snake_game.cpp
)

add_library(game_impl STATIC ${SNAKE_GAME_SOURCES})

target_include_directories(game_impl PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(game_impl PUBLIC
    domain
    graphics
    input
)