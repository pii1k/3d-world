add_library(adapter_in STATIC
    src/simulation_cli_adapter.cpp
)
add_library(ads::adapter::in ALIAS adapter_in)

target_include_directories(adapter_in
PUBLIC
    include
)

target_link_libraries(adapter_in
PUBLIC
    ads::usecase
    ads::port::in
)
