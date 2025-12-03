file(GLOB adapters_sources
  CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)
add_library(adapters_lib STATIC ${adapters_sources})

target_include_directories(adapters_lib
PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

find_package(SFML 2.5 COMPONENTS window system REQUIRED)
find_package(OpenGL REQUIRED)

target_link_libraries(adapters_lib
PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    OpenGL::GL
    core_lib
)
