add_library(domain_model STATIC
    src/vector2.cpp
    src/vehicle.cpp
    src/pedestrian.cpp
    src/world.cpp
)
add_library(ads::domain::model ALIAS domain_model)

target_include_directories(domain_model
PUBLIC
    include
)
