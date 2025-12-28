cmake_minimum_required(VERSION 3.20)
project(autonomous-driving-simulation LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# GLFW
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.4
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_X11 ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# GLAD
FetchContent_Declare(
  glad
  GIT_REPOSITORY https://github.com/Dav1dde/glad.git
  GIT_TAG v2.0.8
)
set(glad_SOURCE_SUBDIR cmake)
FetchContent_MakeAvailable(glad)

FetchContent_GetProperties(glad)
set(GLAD_SOURCES_DIR ${glad_SOURCE_DIR} CACHE PATH "" FORCE)
set(GLAD_CMAKE_DIR ${glad_SOURCE_DIR}/cmake CACHE PATH "" FORCE)
include(${glad_SOURCE_DIR}/cmake/GladConfig.cmake)

if(NOT TARGET glad)
  glad_add_library(glad STATIC
    REPRODUCIBLE
    LANGUAGE C
    API gl:core=4.1
  )
endif()

# GLM
FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm
  GIT_TAG 1.0.1
)
set(glm_SOURCE_SUBDIR cmake)
FetchContent_MakeAvailable(glm)

# Simulator exe
file(GLOB PROJECT_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_include_directories(${PROJECT_NAME}
  PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/third_party
)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
    glad
    glfw
    glm
)

target_compile_options(${PROJECT_NAME}
  PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -O2
)

target_compile_definitions(${PROJECT_NAME}
  PRIVATE
    ASSETS_DIR="${CMAKE_SOURCE_DIR}/assets"
)
