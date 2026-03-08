set(GAME_NAME "snake_game" CACHE STRING "Game to build")

add_subdirectory(graphics)
add_subdirectory(engine)
add_subdirectory(input)
add_subdirectory(domain)

add_executable(main_app app/main.cpp)

target_include_directories(main_app PRIVATE
  "graphics/include"
  "engine/include"
  "input/include"
  "domain/include"
)

target_link_libraries(main_app
PRIVATE
  game_impl
  engine
  input
  graphics
)