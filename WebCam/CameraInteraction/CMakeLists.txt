# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(MyCameraInteraction VERSION 1.0)

# Specify the C++ standard to use
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set the path to the OpenCV installation directory
set(OpenCV_DIR "../opencv/build")

# Find the OpenCV package
find_package(OpenCV REQUIRED)

# Include the OpenCV header directories
include_directories(${OpenCV_INCLUDE_DIRS})

# Set the CMP0079 policy to NEW
cmake_policy(SET CMP0079 NEW)

# Check if the target already exists before creating it
if(NOT TARGET MyCameraInteraction)
    # Add the DLL library
    add_library(MyCameraInteraction SHARED CameraInteraction.cpp CameraInteraction.h)
endif()

# Specify the header directory for the library
target_include_directories(MyCameraInteraction PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link the DLL library with the OpenCV libraries
target_link_libraries(MyCameraInteraction ${OpenCV_LIBS})
