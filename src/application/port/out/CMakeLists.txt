add_library(sim_port_out INTERFACE)
add_library(sim::port::out ALIAS sim_port_out)

target_include_directories(sim_port_out
INTERFACE
    include
)

target_link_libraries(sim_port_out
INTERFACE
    sim::domain::model
)
