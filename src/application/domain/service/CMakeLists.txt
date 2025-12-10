add_library(domain_service STATIC
    src/engine.cpp
)
add_library(ads::domain::service ALIAS domain_service)

target_include_directories(domain_service
PUBLIC
    include
PRIVATE
    src
)

target_link_libraries(domain_service
PRIVATE
    ads::port::out
)