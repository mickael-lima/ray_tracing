cmake_minimum_required(VERSION 3.25)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

enable_testing()

project(ray_tracing)

# Google test suit
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest)


# raytracing build
add_executable(
  ray_tracing

  main.cpp
  src/render.cpp

 "src/vector3d.cpp")

add_executable(
  tests
  tests/vector3d-unittest.cpp
  src/vector3d.cpp
)

target_link_libraries(
  tests
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests)
