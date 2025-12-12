add_library(app STATIC
    src/engine.cpp
)

target_include_directories(app
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(app
PUBLIC
    ecs
    graphics
)