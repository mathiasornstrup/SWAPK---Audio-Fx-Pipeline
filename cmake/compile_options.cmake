add_library(compile_options INTERFACE)
target_compile_options(
  compile_options
  INTERFACE
  -g
  -O3
  -Wall
  -Wextra
  -Wshadow
  -Wnon-virtual-dtor
  -Wold-style-cast
  -Wcast-align
  -Wunused
  -Woverloaded-virtual
  -Wpedantic
  -Wconversion
  -Wsign-conversion
  -Wmisleading-indentation
  -Wnull-dereference
  -Wdouble-promotion
  -Wformat=2
  -Werror
)


target_compile_features(
  compile_options
  INTERFACE
  cxx_std_20
)

