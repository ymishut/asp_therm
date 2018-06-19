#cmake_minimum_required(VERSION 3.5)
add_compile_options(-Wall -std=c++11)

project(lmodels)

#include_directories(${PROJECT_SOURCE_DIR}/common)
#include_directories(${PROJECT_SOURCE_DIR}/target_sys)
#include_directories(${PROJECT_SOURCE_DIR}/gas_parameters)
#include_directories(${PROJECT_SOURCE_DIR}/phase_diagram)
#include_directories(${PROJECT_SOURCE_DIR}/subroutins)

set(
  MODELS_SRC

  #${PROJECT_SOURCE_DIR}/target_sys/target_sys.h
  model_general.cpp
  model_ideal_gas.cpp
  model_peng_robinson.cpp
  model_redlich_kwong.cpp
  models_creator.cpp
)

add_library(${PROJECT_NAME} STATIC ${MODELS_SRC})

target_include_directories( ${PROJECT_NAME}
  PUBLIC ${PROJECT_SOURCE_DIR}/../common)
target_include_directories( ${PROJECT_NAME}
  PUBLIC ${PROJECT_SOURCE_DIR}/../target_sys)
target_include_directories( ${PROJECT_NAME}
  PUBLIC ${PROJECT_SOURCE_DIR}/../gas_parameters)
target_include_directories( ${PROJECT_NAME}
  PUBLIC ${PROJECT_SOURCE_DIR}/../phase_diagram)
target_include_directories( ${PROJECT_NAME}
  PUBLIC ${PROJECT_SOURCE_DIR}/../subroutins)

# link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

target_link_libraries(${PROJECT_NAME} 
    itarget_sys
    lcommon
	lgas_parameters
	lphase_diagram)
#target_link_libraries(models_lib common_lib.lib)
#target_link_libraries(models_lib parameters_lib.lib)
#target_link_libraries(models_lib phase_diagram_lib.lib)