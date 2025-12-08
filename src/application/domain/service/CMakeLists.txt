add_library(domain_service STATIC
    src/simulation_service.cpp
)
add_library(ads::domain::service ALIAS domain_service)

target_include_directories(domain_service
PUBLIC
    include
)

target_link_libraries(domain_service
PUBLIC
    ads::domain::model
)
