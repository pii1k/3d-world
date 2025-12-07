add_library(sim_usecase STATIC
    src/run_simulation_service.cpp
)
add_library(sim::usecase ALIAS sim_usecase)

target_include_directories(sim_usecase
PUBLIC
    include
)

target_link_libraries(sim_usecase
PUBLIC
    sim::domain::service
    sim::port::in
    sim::port::out
)
