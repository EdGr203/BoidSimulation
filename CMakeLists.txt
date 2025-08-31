cmake_minimum_required(VERSION 3.28)
project(BoidSimulation VERSION 0.1.0)

include(CTest)  # Enable testing support

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable warnings
string(APPEND CMAKE_CXX_FLAGS
      " -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion"
      " -Wshadow -Wimplicit-fallthrough -Wextra-semi -Wold-style-cast"
      " -fno-omit-frame-pointer")


if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  string(APPEND CMAKE_CXX_FLAGS " -D_GLIBCXX_ASSERTIONS")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  string(APPEND CMAKE_CXX_FLAGS " -D_LIBCPP_HARDENING_MODE=_LIBCPP_HARDENING_MODE_EXTENSIVE")
endif()

string(APPEND CMAKE_CXX_FLAGS_DEBUG " -fsanitize=address,undefined")
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  string(APPEND CMAKE_CXX_FLAGS " -D_GLIBCXX_SANITIZE_STD_ALLOCATOR")
endif()
string(APPEND CMAKE_EXE_LINKER_FLAGS_DEBUG " -fsanitize=address,undefined")

find_package(SFML 2.6 COMPONENTS graphics REQUIRED)

add_executable(BoidSimulation
    source/boid.cpp
    source/flock.cpp
    source/evolution.cpp
    source/obstacle.cpp
    source/quadtree.cpp
    source/menu.cpp
    source/main.cpp  
)

target_include_directories(BoidSimulation PRIVATE ${CMAKE_SOURCE_DIR}/source)
target_link_libraries(BoidSimulation PRIVATE sfml-graphics)

if (BUILD_TESTING)
  add_executable(test_boid
      testing/test_boid.cpp
      source/boid.cpp
      source/flock.cpp
      source/evolution.cpp
      source/obstacle.cpp
      source/quadtree.cpp
      source/menu.cpp
  )

  target_include_directories(test_boid PRIVATE ${CMAKE_SOURCE_DIR}/source ${CMAKE_SOURCE_DIR})
  target_link_libraries(test_boid PRIVATE sfml-graphics)

add_test(NAME boid_tests COMMAND $<TARGET_FILE:test_boid>)
endif()
