cmake_minimum_required(VERSION 3.20)
project(3d-world LANGUAGES CXX)

find_package(OpenGL REQUIRED)
include(FetchContent)

# glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    glfw
    URL https://github.com/glfw/glfw/archive/refs/tags/3.3.9.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(glfw)

# glm
FetchContent_Declare(
    glm
    URL https://github.com/g-truc/glm/archive/refs/tags/1.0.1.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(glm)

# imgui
FetchContent_Declare(
    imgui
    URL https://github.com/ocornut/imgui/archive/refs/tags/v1.90.4.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(imgui)

add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui
PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

target_link_libraries(imgui
PUBLIC
    glfw
    OpenGL::GL
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src/ecs)
add_subdirectory(src/graphics)
add_subdirectory(src/application)

add_executable(${PROJECT_NAME}
    src/main.cpp
)

target_link_libraries(${PROJECT_NAME}
PRIVATE
    app
)