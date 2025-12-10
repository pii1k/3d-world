add_library(port_out INTERFACE)
add_library(ads::port::out ALIAS port_out)

target_include_directories(port_out
INTERFACE
    interface
)