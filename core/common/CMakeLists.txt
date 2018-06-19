cmake_minimum_required(VERSION 3.5)
#add_compile_options(-Wall -std=c++11)

project(lcommon)

include_directories(${PROJECT_SOURCE_DIR}/target_sys)

set(
  COMMON_SRC

  # common.h
  # ${PROJECT_SOURCE_DIR}/target_sys/target_sys.h
  models_errors.cpp
  models_math.cpp
)

add_library(lcommon STATIC ${COMMON_SRC})
