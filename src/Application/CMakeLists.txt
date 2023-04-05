# Set the project name
project(Application)

# Add the executable
file(GLOB SOURCE_FILES "*.cpp")
file(GLOB UI_FILES "*.ui")
file(GLOB HEADER_FILES "*.h")
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES} ${UI_FILES})

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
					
#Link this project with te runtime output dir
target_link_directories(${PROJECT_NAME} PUBLIC
	${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

target_link_libraries(${PROJECT_NAME} 
	CameraInteraction
	ObjectDetection
	Qt6::Core
	Qt6::Widgets
	${OpenCV_LIBS}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)