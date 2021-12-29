cmake_minimum_required(VERSION 3.10)

project(Plasmatum)

find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})
# execute_process(COMMAND llvm-config --ldflags --libs all OUTPUT_VARIABLE llvm_libs)

set(CMAKE_CXX_COMPILER "clang++")
set(llvm_libs "-L/usr/lib -lLLVM-13")
set(llvm_cxx_flags "-I/usr/include -std=c++14 -fno-exceptions -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS")

file(GLOB PLSM_LIB_SRC "src/lib/*.cc")
file(GLOB MAIN_SRC "src/main/*.cc")
file(GLOB PLSM_RUNTIME_LIB_SRC "src/runtime-lib/*.cc")

include_directories(include)

add_library(plsm-runtime SHARED ${PLSM_RUNTIME_LIB_SRC})
add_executable(plasmatum ${MAIN_SRC} ${PLSM_LIB_SRC})

# llvm_map_components_to_libnames(llvm_libs all)
target_link_libraries(plasmatum ${llvm_libs})
target_link_libraries(plasmatum plsm-runtime)

target_compile_options(plasmatum PRIVATE -O3 -Wall -Wextra ${llvm_cxx_flags})
