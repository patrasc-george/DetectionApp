# Qt 6.2.4 uses CMake 3.16, so that should be our minimum CMake version
cmake_minimum_required(VERSION 3.16)

# Set the name for the project
project(DetectionApp)

# Specify where should the executables and libraries created
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Application)

find_package(OpenCV REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Gui Core Widgets)

# Add the subdirectories for the different parts of the application
add_subdirectory(tests)
add_subdirectory(src/Application)
add_subdirectory(src/ImageProcessingUtils)
add_subdirectory(src/ObjectDetection)
add_subdirectory(src/GUI)
