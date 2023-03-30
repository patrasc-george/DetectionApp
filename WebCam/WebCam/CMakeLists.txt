# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Set the project name
project(WebCam)

# Add subdirectories for the CameraInteraction and ObjectDetection projects
add_subdirectory(../CameraInteraction CameraInteraction)
add_subdirectory(../ObjectDetection ObjectDetection)

# Add an executable for the WebCam project
add_executable(WebCam Source.cpp)

# Set the path to the OpenCV installation directory
set(OpenCV_DIR "../opencv/build")

# Set the path to the CameraInteraction library directory
set(CAMERAINTERACTION_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../x64/Debug)

# Link the WebCam target with the CameraInteraction library
target_link_libraries(WebCam ${CAMERAINTERACTION_LIB_DIR}/CameraInteraction.lib)

# Set the path to the CameraInteraction library directory
set(OBJECTDETECTION_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../x64/Debug)

# Link the WebCam target with the CameraInteraction library
target_link_libraries(WebCam ${OBJECTDETECTION_LIB_DIR}/ObjectDetection.lib)

# Find the OpenCV package
find_package(OpenCV REQUIRED)

# Include the OpenCV header directories
include_directories(${OpenCV_INCLUDE_DIRS})

# Add the OpenCV header directories to the WebCam target
target_include_directories(WebCam PRIVATE ${OpenCV_INCLUDE_DIRS})

# Link the WebCam target with the OpenCV libraries
target_link_libraries(WebCam ${OpenCV_LIBS})

# Set properties for the WebCam target
set_target_properties(WebCam PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Debug)

# Copy the CameraInteraction.dll and ObjectDetection.dll files to the build directory
file(COPY ${CAMERAINTERACTION_LIB_DIR}/CameraInteraction.dll ${OBJECTDETECTION_LIB_DIR}/ObjectDetection.dll ${OpenCV_DIR}/x64/vc16/bin/opencv_world470d.dll
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
