add_library(sim_assembly STATIC
    src/application_builder.cpp
)
add_library(sim::assembly ALIAS sim_assembly)

target_include_directories(sim_assembly
PUBLIC
    include
)

target_link_libraries(sim_assembly
PUBLIC
    sim::adapter::in
    sim::adapter::out
    sim::usecase
    sim::domain::service
    sim::common
)
