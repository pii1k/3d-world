add_library(assembly
    src/application_builder.cpp
)
add_library(ads::assembly ALIAS assembly)

target_include_directories(assembly
PUBLIC
    include
)

target_link_libraries(assembly
PUBLIC
    ads::port::out
)