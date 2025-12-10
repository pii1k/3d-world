cmake_minimum_required(VERSION 3.20)
project(automotive-simulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src/application/domain/service)
add_subdirectory(src/application/port/out)
add_subdirectory(src/assembly)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME}
PRIVATE
    ads::assembly
)