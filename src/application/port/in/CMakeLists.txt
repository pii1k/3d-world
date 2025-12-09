add_library(port_in INTERFACE)
add_library(ads::port::in ALIAS port_in)

target_include_directories(port_in
INTERFACE
    include
)

target_link_libraries(port_in
INTERFACE
    ads::domain::model
)