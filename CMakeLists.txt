# Qt 6.2.4 uses CMake 3.16, so that should be our minimum CMake version
cmake_minimum_required(VERSION 3.16)

# Set the name for the project
project(DetectionApp)

# Specify where should the executables and libraries created
# This helps us avoid copying the files manually or using a post-build command
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

#
set(CMAKE_AUTOMOC ON)

find_package(OpenCV REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Gui Widgets)

# Add the subdirectories for the different parts of the application
add_subdirectory(src/Application)
add_subdirectory(src/CameraInteraction)
add_subdirectory(src/ObjectDetection)

add_subdirectory(src/CameraInteraction_v2)
add_subdirectory(src/ObjectDetection_v2)
add_subdirectory(src/WebCam)
#add_subdirectory(src/UserInterface)
