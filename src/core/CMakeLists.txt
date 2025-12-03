add_library(core_lib STATIC
src/pedestrian.cpp
)

target_include_directories(core_lib
PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

find_package(glm CONFIG REQUIRED)

target_link_libraries(core_lib
PRIVATE
    glm::glm
)