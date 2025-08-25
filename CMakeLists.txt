cmake_minimum_required(VERSION 3.16)
project(BoidSimulation VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Apple-specific settings
if(APPLE)
  set(CMAKE_OSX_DEPLOYMENT_TARGET "13.0" CACHE STRING "macOS deployment target")
  set(CMAKE_OSX_ARCHITECTURES "arm64" CACHE STRING "Build architecture" FORCE)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(
  -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion
  -Wshadow -Wimplicit-fallthrough -Wextra-semi -Wold-style-cast
  -fno-omit-frame-pointer
)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=address,undefined")

set(SOURCES
    source/main.cpp
    source/boid.cpp
    source/flock.cpp
    source/evolution.cpp
    source/obstacle.cpp
    source/quadtree.cpp
    source/menu.cpp
)

# --- Main executable ---
add_executable(BoidSimulation ${SOURCES})

find_package(SFML 2 COMPONENTS graphics window system REQUIRED)
target_link_libraries(BoidSimulation PRIVATE sfml-graphics sfml-window sfml-system)

# --- Tests ---
enable_testing()

add_executable(test_boid
    testing/test_boid.cpp   
    source/boid.cpp
    source/flock.cpp
    source/evolution.cpp
    source/obstacle.cpp
    source/quadtree.cpp
    source/menu.cpp
)

target_link_libraries(test_boid PRIVATE sfml-graphics sfml-window sfml-system)

target_include_directories(test_boid PRIVATE ${CMAKE_SOURCE_DIR}/source)
target_include_directories(BoidSimulation PRIVATE ${CMAKE_SOURCE_DIR}/source)
target_include_directories(test_boid PRIVATE ${CMAKE_SOURCE_DIR})

add_test(NAME boid_tests COMMAND test_boid)
