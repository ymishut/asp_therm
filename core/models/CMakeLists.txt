cmake_minimum_required(VERSION 2.8)
add_compile_options(-Wall -std=c++11)


include_directories(../common)
include_directories(../target)
include_directories(../gas_parameters)
include_directories(../phase_diagram)
include_directories(../subroutins)

set(
  MODELS_SRC

  model_general.cpp
  model_ideal_gas.cpp
  model_peng_robinson.cpp
  model_redlich_kwong.cpp
  models_creator.cpp
)

add_library(models_lib ${MODELS_SRC})
