add_library(ecs INTERFACE)

target_include_directories(ecs
INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/component/include
    ${CMAKE_CURRENT_SOURCE_DIR}/entity/include
    ${CMAKE_CURRENT_SOURCE_DIR}/systems/include
)

target_link_libraries(ecs
INTERFACE
    glm::glm
)