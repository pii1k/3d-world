add_library(sim_adapter_out STATIC
    src/console_render_adapter.cpp
    src/scripted_sensor_adapter.cpp
    src/in_memory_v2x_adapter.cpp
)
add_library(sim::adapter::out ALIAS sim_adapter_out)

target_include_directories(sim_adapter_out
PUBLIC
    include
)

target_link_libraries(sim_adapter_out
PUBLIC
    sim::port::out
    sim::domain::model
)
