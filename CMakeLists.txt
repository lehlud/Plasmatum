cmake_minimum_required(VERSION 3.10)

project(Plasmatum)

set(CMAKE_CXX_COMPILER "clang++")

file(GLOB PLSM_SRC
    "src/*.cc"
)

include_directories(include)
add_executable(plsm ${PLSM_SRC})
target_compile_options(plsm PRIVATE -O3 -Wall -Wextra)
target_link_options(plsm PRIVATE -flto)