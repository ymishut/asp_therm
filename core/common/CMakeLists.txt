cmake_minimum_required(VERSION 2.8)
add_compile_options(-Wall -std=c++11)

set(
  COMMON_SRC

  common.h
  models_errors.cpp
  models_math.cpp
)

add_library(common_lib ${COMMON_SRC})
