cmake_minimum_required(VERSION 2.8)
add_compile_options(-Wall -std=c++11)

include_directories(../common)
include_directories(../target)

set(
  SUBROUTINS_SRC
  
  file_reading.cpp
#  inputdata_by_file.cpp
#  load_config.cpp
  xml_reader.cpp

#  models_output.h
)

add_library(subroutins_lib  ${SUBROUTINS_SRC})
