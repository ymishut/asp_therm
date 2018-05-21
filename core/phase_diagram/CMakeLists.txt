cmake_minimum_required(VERSION 2.8)
add_compile_options(-Wall -std=c++11)

include_directories(../common)
include_directories(../target)
include_directories(../gas_parameters)
include_directories(../subroutins)

set(
  PHASE_DIAGRAM_SRC

  phase_diagram.cpp
  phase_diagram_models.cpp
)

add_library(phase_diagram_lib ${PHASE_DIAGRAM_SRC})
