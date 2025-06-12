### ----------------------------------------------------------------------------------
### shared stuff
### ----------------------------------------------------------------------------------

### enforce version 3.20+
cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

### we use C++23
set(CMAKE_CXX_STANDARD 23)

### convenient for LSPs
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

### external deps/libs
add_subdirectory(thirdparty/spdlog)
add_subdirectory(thirdparty/argparse)
add_subdirectory(thirdparty/magic_enum)
add_subdirectory(thirdparty/ArenaAllocator)

### ----------------------------------------------------------------------------------
### libsirf stuff
### ----------------------------------------------------------------------------------

### libsirf declaration
project(sirf LANGUAGES CXX)

### libsirf sources
file(GLOB_RECURSE SOURCES libsirf/*.cpp)

### libsirf library declaration
add_library(sirf STATIC ${SOURCES})

### libsirf library stuff
target_link_libraries(sirf PRIVATE magic_enum)
target_link_libraries(sirf PRIVATE ArenaAllocator)

### libsirf include dirs
target_include_directories(sirf PUBLIC
  include
  include/libsirf
  libsirf
)

### ----------------------------------------------------------------------------------
### sirfc stuff
### ----------------------------------------------------------------------------------

### sirfc declaration
project(sirfc LANGUAGES CXX)

### sirfc sources
file(GLOB_RECURSE SIRFC_SOURCES sirfc/*.cpp)

### sirfc executable declaration
add_executable(sirfc ${SIRFC_SOURCES})

target_link_libraries(sirfc PRIVATE sirf)
target_link_libraries(sirfc PRIVATE spdlog)
target_link_libraries(sirfc PRIVATE argparse)
target_link_libraries(sirfc PRIVATE magic_enum)
target_link_libraries(sirfc PRIVATE ArenaAllocator)

### sirfc include dirs
target_include_directories(sirfc PUBLIC
  include
  include/libsirf
)

### ----------------------------------------------------------------------------------
### more global crap
### ----------------------------------------------------------------------------------

### alias stuff
add_library(sirf::sirf ALIAS sirf)
add_executable(sirf::sirfc ALIAS sirfc)
