cmake_minimum_required(VERSION 2.8)
add_compile_options(-Wall -std=c++11)

set(
  SUBROUTINS_SRC
  
  file_reading.cpp
  inputdata_by_file.cpp
  load_config.cpp
)

add_library(subroutins_lib  ${SUBROUTINS_SRC})
