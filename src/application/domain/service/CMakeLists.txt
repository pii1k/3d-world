add_library(sim_domain_service STATIC
    src/simulation_service.cpp
)
add_library(sim::domain::service ALIAS sim_domain_service)

target_include_directories(sim_domain_service
PUBLIC
    include
)

target_link_libraries(sim_domain_service
PUBLIC
    sim::domain::model
)
