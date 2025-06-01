cmake_minimum_required(VERSION 3.20)

project(sirf LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SOURCES src/*.cpp)

add_library(sirf STATIC ${SOURCES})

target_include_directories(sirf PUBLIC
  include
  include/SIRF
)

add_executable(sirfc sirfc/main.cpp)

add_subdirectory(thirdparty/spdlog)
add_subdirectory(thirdparty/argparse)

target_link_libraries(sirfc PRIVATE spdlog::spdlog)
target_link_libraries(sirfc PRIVATE argparse)
target_link_libraries(sirfc PRIVATE sirf)

target_include_directories(sirfc PUBLIC
  include
  include/SIRF
)
