add_library(sim_common STATIC
    src/scope_guard.cpp
)
add_library(sim::common ALIAS sim_common)

target_include_directories(sim_common
PUBLIC
    include
PRIVATE
    src
)
