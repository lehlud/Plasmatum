cmake_minimum_required(VERSION 3.10)

project(Plasmatum)

set(CMAKE_CXX_COMPILER "clang++")

file(GLOB PLSM_LIB_SRC "src/lib/*.cc")
file(GLOB MAIN_SRC "src/main/*.cc")

include_directories(include)

add_library(plsmlib STATIC ${PLSM_LIB_SRC})

add_executable(plsm ${MAIN_SRC})
target_link_libraries(plsm plsmlib)
target_link_options(plsm PRIVATE -flto)
target_compile_options(plsm PRIVATE -O3 -Wall -Wextra)