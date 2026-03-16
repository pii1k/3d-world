project(domain)

# This library is header-only, but we create an INTERFACE library
# to make this include directory available to other targets.
add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${GAME_NAME}")
    add_subdirectory(${GAME_NAME})
else()
    message(FATAL_ERROR "Game '${GAME_NAME}' not found.")
endif()