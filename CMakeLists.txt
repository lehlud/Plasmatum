cmake_minimum_required(VERSION 3.10)

project(Plasmatum)

set(CMAKE_CXX_COMPILER "clang++")

file(GLOB PLSM_LIB_SRC "src/lib/*.cc")
file(GLOB MAIN_SRC "src/main/*.cc")

include_directories(include)

add_library(plsm STATIC ${PLSM_LIB_SRC})

add_executable(plasmatum ${MAIN_SRC})
target_link_libraries(plasmatum plsm)
target_link_options(plasmatum PRIVATE -flto)
target_compile_options(plasmatum PRIVATE -O3 -Wall -Wextra)