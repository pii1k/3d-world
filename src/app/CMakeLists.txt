add_executable(ADS main.cpp)

target_link_libraries(ADS
PRIVATE
    adapters_lib
    core_lib
)

set_target_properties(ADS PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)