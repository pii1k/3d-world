add_library(common STATIC
    src/scope_guard.cpp
)
add_library(ads::common ALIAS common)

target_include_directories(common
PUBLIC
    include
PRIVATE
    src
)
