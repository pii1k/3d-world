cmake_minimum_required(VERSION 3.20)
project(autonomous-driving-simulation LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
if(NOT glad_FOUND)
  FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG v2.0.8
  )
  set(glad_SOURCE_SUBDIR cmake)
  FetchContent_MakeAvailable(glad)
  set(GLAD_SOURCES_DIR ${glad_SOURCE_DIR} CACHE STRING "" FORCE)
  include(${glad_SOURCE_DIR}/cmake/GladConfig.cmake)
  glad_add_library(glad STATIC API gl:core=3.3)
endif()

add_subdirectory(src/core)
add_subdirectory(src/adapters)
add_subdirectory(src/app)