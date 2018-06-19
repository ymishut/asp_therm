cmake_minimum_required(VERSION 3.5)
add_compile_options(-Wall -std=c++11)
if(MSVC)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

project(asp_therm)

message(STATUS "source_dir is \n")
message(STATUS ${PROJECT_SOURCE_DIR})

set(CMAKE_LIBARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../../temp-builds/core_build_libs/")
set(LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/../../temp-builds/core_build_libs/")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4514")

#include_directories(common)
#include_directories(${PROJECT_SOURCE_DIR}/gas_parameters)
#include_directories(${PROJECT_SOURCE_DIR}/models)
#include_directories(phase_diagram)
#include_directories(subroutins)
#include_directories(target)

#target_include_directories( ${PROJECT_NAME}
#  PUBLIC ${PROJECT_SOURCE_DIR}/gas_parameters)
#target_include_directories( ${PROJECT_NAME}
#  PUBLIC ${PROJECT_SOURCE_DIR}/models)

add_subdirectory(common)
add_subdirectory(target_sys)
add_subdirectory(gas_parameters)
add_subdirectory(models)
add_subdirectory(phase_diagram)
add_subdirectory(subroutins)

add_executable(${PROJECT_NAME} main.cpp)
#link_directories(${CMAKE_SOURCE_DIR}/../../temp-builds/core_build_libs)
target_link_libraries(${PROJECT_NAME} 
    lgas_parameters
    lmodels
)
#target_link_libraries(${PROJECT_NAME} 
#)
#target_link_libraries(${PROJECT_NAME} models_lib.lib)