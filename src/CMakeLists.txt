cmake_minimum_required(VERSION 3.11-3.18)

project(src)

set(SDL2_DIR "C:\\Users\\Cameron\\Desktop\\SDL2-2.28.5")
#set(SDL2_image_DIR "C:\\Users\\Cameron\\Desktop\\SDL2-2.28.5\\cmake")
find_package(SDL2 REQUIRED)
#find_package(SDL_ttf REQUIRED)
#find_package(SDL2_image REQUIRED)

add_library(${PROJECT_NAME} 
    classes/BezierPath.cpp 
    classes/Button.cpp 
    classes/Car.cpp 
    classes/CarHandler.cpp 
    classes/Infrastructure.cpp 
    classes/Intersection.cpp 
    classes/Road.cpp 
    classes/Router.cpp 
    classes/Scene.cpp 
    classes/System.cpp
    classes/AStar.cpp
    classes/Node.cpp)


target_include_directories(${PROJECT_NAME}
    PUBLIC ${SDL2_INCLUDE_DIRS}
    PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})

# Tell compiler to use C++20 features.
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
