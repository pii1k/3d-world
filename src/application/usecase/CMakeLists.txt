add_library(usecase STATIC
    src/run_simulation_service.cpp
)
add_library(ads::usecase ALIAS usecase)

target_include_directories(usecase
PUBLIC
    include
)

target_link_libraries(usecase
PUBLIC
    ads::domain::service
    ads::port::in
    ads::port::out
)
