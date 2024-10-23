cmake_minimum_required(VERSION 3.29)
project(dpfp_test)

set(CMAKE_CXX_STANDARD 17)

# Include the SDK headers
include_directories("sdk/include")

# Add the executable
add_executable(${PROJECT_NAME} main.cpp)

# Link the library directory with PRIVATE visibility
target_link_directories(${PROJECT_NAME} PRIVATE "sdk/lib/")

# Link the required libraries
target_link_libraries(${PROJECT_NAME} "DPFPApi.lib" "DPFpUI.lib" "dpHFtrEx.lib" "dpHMatch.lib")

# Enable verbose linker output
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -v")