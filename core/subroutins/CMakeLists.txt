# cmake_minimum_required(VERSION 3.5)
add_compile_options(-Wall -std=c++11)

project(lsubroutins)

set(
  SUBROUTINS_SRC
  
  file_reading.cpp
#  inputdata_by_file.cpp
#  load_config.cpp
  #xml_reader.cpp
#  models_output.h
)

add_library(${PROJECT_NAME} STATIC ${SUBROUTINS_SRC})

target_include_directories( ${PROJECT_NAME}
  PUBLIC ${PROJECT_SOURCE_DIR}/../common)
target_include_directories( ${PROJECT_NAME}
  PUBLIC ${PROJECT_SOURCE_DIR}/../target_sys)
target_include_directories( ${PROJECT_NAME}
  PUBLIC ${PROJECT_SOURCE_DIR}/../gas_parameters)

#include_directories(${PROJECT_SOURCE_DIR}/target_sys)
#include_directories(${PROJECT_SOURCE_DIR}/gas_parameters)

link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

target_link_libraries(${PROJECT_NAME} 
    itarget_sys
    lcommon
    lgas_parameters
)
