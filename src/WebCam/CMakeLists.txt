# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Set the project name
project(WebCam)

# Add an executable for the WebCam project
add_executable(WebCam Source.cpp)

# Set the path to the OpenCV installation directory
set(OpenCV_DIR "../opencv/build")

# Find the OpenCV package
find_package(OpenCV REQUIRED)

# Include the OpenCV header directories
include_directories(${OpenCV_INCLUDE_DIRS})

# Add the OpenCV header directories to the WebCam target
target_include_directories(WebCam PRIVATE ${OpenCV_INCLUDE_DIRS})

# Set the directory where the linker should look for the libraries needed
target_link_directories(WebCam PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../x64/Debug)

# Link the required libraries
target_link_libraries(WebCam 
    CameraInteraction
    ObjectDetection
    ${OpenCV_LIBS}
)

# A custom command to be executed after the project is built to copy the .dll files
add_custom_command(TARGET WebCam POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/../x64/Debug/CameraInteraction.dll
        ${CMAKE_CURRENT_SOURCE_DIR}/../x64/Debug/ObjectDetection.dll
        ${CMAKE_CURRENT_SOURCE_DIR}/../x64/Debug/opencv_world470d.dll
        $<TARGET_FILE_DIR:WebCam>
)
