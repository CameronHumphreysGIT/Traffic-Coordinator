cmake_minimum_required(VERSION 3.11-3.18)
project(test)
add_definitions(-DUNICODE -D_UNICODE)
find_package(Catch2 3 REQUIRED)
# These tests can use the Catch2-provided main
add_executable(tests test.cpp)
target_link_libraries(tests PUBLIC Catch2::Catch2WithMain)
# These tests need their own main
#add_executable(custom-main-tests test.cpp test-main.cpp)
#target_link_libraries(custom-main-tests PRIVATE Catch2::Catch2)
#include(CTest)
#include(Catch)
#catch_discover_tests(tests)