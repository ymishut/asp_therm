#cmake_minimum_required(VERSION 3.5)
add_compile_options(-Wall -std=c++11)

project(lphase_diagram)
#include_directories(${PROJECT_SOURCE_DIR}/common)
#include_directories(${PROJECT_SOURCE_DIR}/target_sys)
#include_directories(${PROJECT_SOURCE_DIR}/gas_parameters)
#include_directories(${PROJECT_SOURCE_DIR}/subroutins)

set(
  PHASE_DIAGRAM_SRC

  phase_diagram.cpp
  phase_diagram_models.cpp
)

add_library(${PROJECT_NAME} STATIC ${PHASE_DIAGRAM_SRC})

target_include_directories( ${PROJECT_NAME}
  PUBLIC ${PROJECT_SOURCE_DIR}/../common)
target_include_directories( ${PROJECT_NAME}
  PUBLIC ${PROJECT_SOURCE_DIR}/../target_sys)
target_include_directories( ${PROJECT_NAME}
  PUBLIC ${PROJECT_SOURCE_DIR}/../gas_parameters)
target_include_directories( ${PROJECT_NAME}
  PUBLIC ${PROJECT_SOURCE_DIR}/../subroutins)

link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

target_link_libraries(${PROJECT_NAME} 
    itarget_sys
    lcommon
	lgas_parameters
)