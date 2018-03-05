cmake_minimum_required(VERSION 2.8)
add_compile_options(-Wall -std=c++11)

set(
  PARAMETERS_SRC

  gas_description.cpp
  gas_description_dynamic.cpp
  gas_description_static.cpp
)

add_library(parameters_lib ${PARAMETERS_SRC})
