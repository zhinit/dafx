cmake_minimum_required(VERSION 3.24)
project(dafx VERSION 0.0.1)

set (CMAKE_CXX_STANDARD 20)

add_executable(dsp-wasm bindings.cpp)

target_link_options(dsp-wasm PRIVATE --bind)

set_target_properties(
  dsp-wasm
  PROPERTIES
    OUTPUT_NAME "dsp-emscripten-glue"
    SUFFIX ".js"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../frontend/public"
)
