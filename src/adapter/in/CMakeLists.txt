add_library(sim_adapter_in STATIC
    src/simulation_cli_adapter.cpp
)
add_library(sim::adapter::in ALIAS sim_adapter_in)

target_include_directories(sim_adapter_in
PUBLIC
    include
)

target_link_libraries(sim_adapter_in
PUBLIC
    sim::usecase
    sim::port::in
)
