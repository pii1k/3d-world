add_library(adapter_in STATIC
    src/controller_opengl.cpp
)
add_library(ads::adapter::in ALIAS adapter_in)

target_include_directories(adapter_in
PUBLIC
    include
)
