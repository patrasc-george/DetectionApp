# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Set the project name
project(WebCamSolution)

# Add subdirectories for the CameraInteraction, ObjectDetection, and WebCam projects
add_subdirectory(CameraInteraction)
add_subdirectory(ObjectDetection)
add_subdirectory(WebCam)