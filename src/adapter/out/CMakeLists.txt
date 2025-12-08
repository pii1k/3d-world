add_library(adapter_out STATIC
    src/gl_render_adapter.cpp
    src/scripted_sensor_adapter.cpp
    src/v2x_adapter.cpp
)
add_library(ads::adapter::out ALIAS adapter_out)

target_include_directories(adapter_out
PUBLIC
    include
)

find_package(SFML 2.5 COMPONENTS window system REQUIRED)
find_package(OpenGL REQUIRED)

target_link_libraries(adapter_out
PUBLIC
    ads::port::out
    ads::domain::model
    sfml-graphics
    sfml-window
    sfml-system
    OpenGL::GL
)
