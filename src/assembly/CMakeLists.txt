add_library(assembly STATIC
    src/application_builder.cpp
)
add_library(ads::assembly ALIAS assembly)

target_include_directories(assembly
PUBLIC
    include
)

target_link_libraries(assembly
PUBLIC
    ads::adapter::in
    ads::adapter::out
    ads::usecase
    ads::domain::service
    ads::common
)
