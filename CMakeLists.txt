cmake_minimum_required(VERSION 3.20)
project(automotive-simulator LANGUAGES CXX)

find_package(OpenGL REQUIRED)
include(FetchContent)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    glfw
    URL https://github.com/glfw/glfw/archive/refs/tags/3.3.9.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(glfw)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(ecs INTERFACE)
target_include_directories(ecs INTERFACE src)

add_library(graphics INTERFACE)
target_include_directories(graphics INTERFACE src)
target_link_libraries(graphics INTERFACE OpenGL::GL)
target_compile_definitions(graphics INTERFACE SHADER_ASSET_DIR="${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/shaders")

add_library(app STATIC
    src/application/engine.cpp
    src/graphics/renderer.cpp
)

target_include_directories(app PUBLIC src)
target_link_libraries(app
PUBLIC
    ecs
    graphics
    glfw
)

add_executable(${PROJECT_NAME}
    src/main.cpp
)
target_link_libraries(${PROJECT_NAME} PRIVATE app)
