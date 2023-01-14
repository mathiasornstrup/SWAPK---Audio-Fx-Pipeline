cmake_minimum_required(VERSION 3.10)
project(EAMFUP VERSION 1.0)

add_executable(
  EAMFUP
  main.cpp
)

include(cmake/compile_options.cmake)
target_include_directories(
  EAMFUP PUBLIC
  "${PROJECT_BINARY_DIR}"
  "${PROJECT_SOURCE_DIR}/include"
)

target_link_libraries(EAMFUP PUBLIC sndfile compile_options)


