cmake_minimum_required(VERSION 2.8)
add_compile_options(-std=c++11 -Wall)

set(PROJECT_NAME swf)
project(${PROJECT_NAME})

include_directories(core/common)
include_directories(core/phase_diagram)
include_directories(core/subroutins)
include_directories(core/models)

find_package(Boost REQUIRED)


#add_executable(${PROJECT_NAME} src/main.cpp)
add_subdirectory(core)
add_subdirectory(flow_modeling)
#target_link_libraries(${PROJECT_NAME} )
