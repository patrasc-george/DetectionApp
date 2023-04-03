# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Set the project name
project(WebCam)

# Add an executable for the WebCam project
add_executable(WebCam Source.cpp)

# Add the OpenCV header directories to the WebCam target
target_include_directories(WebCam PRIVATE ${OpenCV_INCLUDE_DIRS})

# Set the directory where the linker should look for the libraries needed
target_link_directories(WebCam PRIVATE 
	${CMAKE_CURRENT_SOURCE_DIR}/../x64/Debug
	${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

# Link the required libraries
target_link_libraries(WebCam 
    MyCameraInteraction
    MyObjectDetection
    ${OpenCV_LIBS}
)
