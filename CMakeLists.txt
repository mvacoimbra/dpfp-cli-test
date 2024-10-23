cmake_minimum_required(VERSION 3.29)
project(dpfp_test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_VERBOSE_MAKEFILE ON)


# Define SDK path as a variable
set(DPFP_SDK_DIR "C:/Program Files/DigitalPersona/One Touch SDK/C-C++/")

# Include the SDK headers
include_directories("${DPFP_SDK_DIR}/Include")

# Add the executable
add_executable(${PROJECT_NAME} main.cpp)

# Link the library directory with PRIVATE visibility
target_link_directories(${PROJECT_NAME} PRIVATE "${DPFP_SDK_DIR}/Lib/x64/")

# Link the required libraries
target_link_libraries(${PROJECT_NAME} "DPFPApi.lib" "DPFpUI.lib" "dpHFtrEx.lib" "dpHMatch.lib")

# Enable verbose linker output
set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -v")