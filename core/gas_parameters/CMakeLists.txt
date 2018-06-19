cmake_minimum_required(VERSION 3.5)
#add_compile_options(-Wall -std=c++11)

project(lgas_parameters)

#include_directories(${PROJECT_SOURCE_DIR}/common)
#include_directories(${PROJECT_SOURCE_DIR}/target_sys)
#include_directories(${PROJECT_SOURCE_DIR}/phase_diagram)
#include_directories(${PROJECT_SOURCE_DIR}/models)

set(
  PARAMETERS_SRC

  examples.cpp
  gas_description.cpp
  gas_description_dynamic.cpp
  gas_description_static.cpp
 # gas_dynamic_update.cpp
  gas_mix_init.cpp
  init_by_file.cpp
)

add_library(lgas_parameters STATIC ${PARAMETERS_SRC})
#[[
target_include_directories( ${PROJECT_NAME}
  PUBLIC ${PROJECT_SOURCE_DIR}/../common)
target_include_directories( ${PROJECT_NAME}
  PUBLIC ${PROJECT_SOURCE_DIR}/../target_sys)
target_include_directories( ${PROJECT_NAME}
  PUBLIC ${PROJECT_SOURCE_DIR}/../phase_diagram)
  ]]
target_include_directories( ${PROJECT_NAME}
  PUBLIC ${PROJECT_SOURCE_DIR}/../models)
#link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
target_link_libraries(${PROJECT_NAME}
    itarget_sys
    lcommon
	lmodels)
#target_link_libraries(parameters_lib common.lib)
#target_link_libraries(parameters_lib phase_diagram_lib.lib)
#target_link_libraries(parameters_lib models_lib.lib)
