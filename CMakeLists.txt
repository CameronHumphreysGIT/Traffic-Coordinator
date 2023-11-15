cmake_minimum_required(VERSION 3.10)

project(Traffic-Coordinator)
#set(SDL2_DIR "C:\\Users\\Cameron\\Desktop\\SDL2-2.28.5")
#find_package(SDL2 REQUIRED)

add_subdirectory(src)
add_executable(main src/main.cpp)
target_link_libraries(main
    PUBLIC src)

#custom command copies the necessary dll and images over to build/Release:
add_custom_command(TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/libs"
        $<TARGET_FILE_DIR:main>)

#add_definitions(-DUNICODE -D_UNICODE)
find_package(Catch2 3 REQUIRED)

#compile and link the tests.exe
# These tests can use the Catch2-provided main
add_executable(tests ${PROJECT_SOURCE_DIR}/test/test.cpp)
target_link_libraries(tests 
    PUBLIC Catch2::Catch2WithMain
    PUBLIC src)

#custom command copies the necessary dll and images over to build/test/:
#message("doing custom command")
#add_custom_command(TARGET tests POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_directory
#        "${PROJECT_SOURCE_DIR}/libs"
#        $<TARGET_FILE_DIR:tests>)
#
target_compile_features(main PUBLIC cxx_std_20)
target_compile_features(tests PUBLIC cxx_std_20)
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
#set(SDL2_DIR "C:\\Users\\Cameron\\Desktop\\SDL2-2.28.5") 
#
#find_package(SDL2 REQUIRED)
##find_package(SDL2_image REQUIRED)
#include_directories(
#    ${SDL2_INCLUDE_DIRS}/include
#    ${PROJECT_SOURCE_DIR}/include
#    ${PROJECT_SOURCE_DIR}/src/main/include)
##include_directories(${SDL2_IMAGE_INCLUDE_DIRS})
#
#add_executable(Traffic-Coordinator src/main/main.cpp)
#
##add the classes library
#add_library(classes 
#    src/main/classes/BezierPath.cpp 
#    src/main/classes/Button.cpp 
#    src/main/classes/Car.cpp 
#    src/main/classes/CarHandler.cpp 
#    src/main/classes/Infrastructure.cpp 
#    src/main/classes/Intersection.cpp 
#    src/main/classes/Road.cpp 
#    src/main/classes/Router.cpp 
#    src/main/classes/Scene.cpp 
#    src/main/classes/System.cpp)
#
#
#target_link_libraries(${PROJECT_NAME} 
#    ${SDL2_LIBRARIES}
#    classes)
#
#target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)