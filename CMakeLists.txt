cmake_minimum_required(VERSION 3.20)

project(SIRF LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SOURCES src/*.cpp)

add_library(SIRF STATIC ${SOURCES})

target_include_directories(SIRF PUBLIC
  include
  include/SIRF
)
