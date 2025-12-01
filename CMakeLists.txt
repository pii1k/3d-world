cmake_minimum_required(VERSION 3.20)
project(autonomous-driving-simulation LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

find_package(SFML 2.5 COMPONENTS window system REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(Python COMPONENTS Interpreter REQUIRED)
find_package(glad CONFIG QUIET)

if(NOT glad_FOUND)
  FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG v2.0.8
  )
  set(glad_SOURCE_SUBDIR cmake)
  FetchContent_MakeAvailable(glad)
endif()

if(NOT TARGET glad::glad)
  if(NOT DEFINED glad_SOURCE_DIR)
    message(
      FATAL_ERROR
        "glad dependency not found. Install a glad package that provides glad::glad or allow FetchContent to download it."
    )
  endif()

  set(GLAD_GENERATED_DIR "${CMAKE_BINARY_DIR}/glad")
  file(MAKE_DIRECTORY "${GLAD_GENERATED_DIR}")

  execute_process(
    COMMAND
      ${CMAKE_COMMAND} -E env "PYTHONPATH=${glad_SOURCE_DIR}"
      ${Python_EXECUTABLE} -m glad --out-path "${GLAD_GENERATED_DIR}" --api gl:core=3.3 --reproducible c
    WORKING_DIRECTORY "${glad_SOURCE_DIR}"
    RESULT_VARIABLE GLAD_GEN_RESULT
  )

  if(NOT GLAD_GEN_RESULT EQUAL 0)
    message(
      FATAL_ERROR
        "glad generation failed (python3 -m glad). Ensure python is available and ${glad_SOURCE_DIR} is accessible."
    )
  endif()

  add_library(glad STATIC "${GLAD_GENERATED_DIR}/src/gl.c")
  target_include_directories(glad PUBLIC "${GLAD_GENERATED_DIR}/include")
  add_library(glad::glad ALIAS glad)
endif()

file(GLOB PROJECT_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_include_directories(${PROJECT_NAME}
  PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    OpenGL::GL
    glm::glm
    glad::glad
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
