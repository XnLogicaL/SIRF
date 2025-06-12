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

## CLI options
option(DEBUG_MODE "Debug mode" ON)

### ----------------------------------------------------------------------------------
### libsirf stuff
### ----------------------------------------------------------------------------------

### libsirf declaration
project(libsirf LANGUAGES CXX)

### libsirf sources
file(GLOB_RECURSE SOURCES libsirf/*.cpp)

### libsirf library declaration
add_library(libsirf STATIC ${SOURCES})

### libsirf library stuff
target_link_libraries(libsirf PRIVATE magic_enum)
target_link_libraries(libsirf PRIVATE ArenaAllocator)

### libsirf include dirs
target_include_directories(libsirf PUBLIC
  include
  include/libsirf
  libsirf
)

if(DEBUG_MODE)
  target_compile_options(libsirf PUBLIC
    -UNDEBUG
    -Og
  )
endif()

### ----------------------------------------------------------------------------------
### sirfc stuff
### ----------------------------------------------------------------------------------

### sirfc declaration
project(sirfc LANGUAGES CXX)

### sirfc sources
file(GLOB_RECURSE SIRFC_SOURCES sirfc/*.cpp)

### sirfc executable declaration
add_executable(sirfc ${SIRFC_SOURCES})

target_link_libraries(sirfc PRIVATE libsirf)
target_link_libraries(sirfc PRIVATE spdlog)
target_link_libraries(sirfc PRIVATE argparse)
target_link_libraries(sirfc PRIVATE magic_enum)
target_link_libraries(sirfc PRIVATE ArenaAllocator)

### sirfc include dirs
target_include_directories(sirfc PUBLIC
  include
  include/libsirf
)

if(DEBUG_MODE)
  target_compile_options(sirfc PUBLIC
    -UNDEBUG
    -Og
  )
endif()

### ----------------------------------------------------------------------------------
### more global crap
### ----------------------------------------------------------------------------------

### alias stuff
add_library(sirf::libsirf ALIAS libsirf)
add_executable(sirf::sirfc ALIAS sirfc)
