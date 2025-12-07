add_library(sim_port_in INTERFACE)
add_library(sim::port::in ALIAS sim_port_in)

target_include_directories(sim_port_in
INTERFACE
    include
)
