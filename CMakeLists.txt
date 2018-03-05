cmake_minimum_required(VERSION 2.8)
add_compile_options(-std=c++11 -Wall)

set(PROJECT_NAME Equations_of_state)
project(${PROJECT_NAME})

find_package(Boost REQUIRED)

set (
  EOF_SOURCES
  src/dynamic_modeling.cpp
  src/dynamic_modeling_diffequations.cpp
  src/dynamic_modeling_diffequationsIN.cpp
  src/dynamic_modeling_diffequationsOUT.cpp
  src/dynamic_modeling_models.cpp
  src/filereading.cpp
  src/gas_description.cpp
  src/gas_description_dynamic.cpp
  src/gas_description_static.cpp
  src/inputdata_by_file.cpp
  src/load_config.cpp
  src/model_general.cpp
  src/model_ideal_gas.cpp
  src/model_peng_robinson.cpp
  src/model_redlich_kwong.cpp
  src/models_creator.cpp
  src/phase_diagram.cpp
  src/phase_diagram_models.cpp
)

add_executable(${PROJECT_NAME} ${EOF_SOURCES} src/main.cpp)
target_link_libraries(${PROJECT_NAME})
