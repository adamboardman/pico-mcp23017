cmake_minimum_required(VERSION 3.12)

add_definitions(-DMOCK_PICO_PI)

project(tests)

set(CMAKE_CXX_STANDARD 20)

find_package(Catch2 REQUIRED)

include_directories(../api)

add_executable(tests test_mcp23017.cpp pico_pi_mocks.cpp ../source/mcp23017.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(tests)