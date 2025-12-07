add_library(sim_domain_model STATIC
    src/vector2.cpp
    src/vehicle.cpp
    src/pedestrian.cpp
    src/world.cpp
)
add_library(sim::domain::model ALIAS sim_domain_model)

target_include_directories(sim_domain_model
PUBLIC
    include
)
