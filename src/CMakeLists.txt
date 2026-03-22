set(GAME_NAME "snake_game" CACHE STRING "Game to build")

add_subdirectory(graphics)
add_subdirectory(engine)
add_subdirectory(domain)
add_subdirectory(util)

add_executable(main_app app/main.cpp)
target_link_libraries(main_app
PRIVATE
  game_impl
)