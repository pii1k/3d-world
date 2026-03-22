cmake_minimum_required(VERSION 3.20)
project(snake_game LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(TARGET_OS "" CACHE STRING "Target platform: mac, windows, or linux")
if(TARGET_OS STREQUAL "")
  if(APPLE)
    set(TARGET_OS "mac" CACHE STRING "Target platform: mac, windows, or linux" FORCE)
  elseif(WIN32)
    set(TARGET_OS "windows" CACHE STRING "Target platform: mac, windows, or linux" FORCE)
  elseif(UNIX)
    set(TARGET_OS "linux" CACHE STRING "Target platform: mac, windows, or linux" FORCE)
  else()
    message(FATAL_ERROR "Unable to infer target OS. Pass -DTARGET_OS=<mac|windows|linux>.")
  endif()
endif()
set_property(CACHE TARGET_OS PROPERTY STRINGS mac windows linux)

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
if(TARGET_OS STREQUAL "linux")
  set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_X11 ON CACHE BOOL "" FORCE)
else()
  set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_X11 OFF CACHE BOOL "" FORCE)
endif()
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

add_subdirectory(src)