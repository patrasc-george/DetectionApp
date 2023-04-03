# Set the project name
project(Application)

# Add the executable
file(GLOB SOURCE_FILES "*.cpp")
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Add the libraries
add_dependencies(${PROJECT_NAME}
	CameraInteraction
	ObjectDetection
)

# Allow the headers to be included in main.cpp
target_include_directories(${PROJECT_NAME} PUBLIC
                          "${CMAKE_SOURCE_DIR}/src/CameraInteraction"
						  "${CMAKE_SOURCE_DIR}/src/ObjectDetection"
						   ${OpenCV_INCLUDE_DIRS}
                          )
					
#Link this project with the runtime output dir
target_link_directories(${PROJECT_NAME} PUBLIC
	${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

target_link_libraries(${PROJECT_NAME} 
	CameraInteraction
	ObjectDetection
	${OpenCV_LIBS}
)
