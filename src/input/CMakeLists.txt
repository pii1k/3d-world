project(input)

set(INPUT_SOURCES
    src/camera_controller.cpp
    src/input_controller.cpp
)

add_library(${PROJECT_NAME} STATIC ${INPUT_SOURCES})

target_include_directories(${PROJECT_NAME}
PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

# camera_controller needs to know about the camera from the engine
target_link_libraries(${PROJECT_NAME}
PUBLIC
    engine
)

target_link_libraries(${PROJECT_NAME}
PRIVATE
    glfw
)