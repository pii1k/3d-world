set(INPUT_SOURCES
    src/camera_controller.cpp
    src/input_controller.cpp
)

add_library(input STATIC ${INPUT_SOURCES})

target_include_directories(input PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

# camera_controller needs to know about the camera from the engine
target_link_libraries(input PUBLIC
    engine
    glfw
)