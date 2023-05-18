# Set the project name
project(Application)

# Add the executable
file(GLOB SOURCE_FILES "*.cpp")
file(GLOB HEADER_FILES "*.h")

set(CMAME_AUTORCC ON)
qt6_add_resources(RES "../../resources.qrc")
add_executable(${PROJECT_NAME} ${RES} ${HEADER_FILES} ${SOURCE_FILES})

# Add the libraries
add_dependencies(${PROJECT_NAME}
	ObjectDetection
	UserInterface
)

# Allow the headers to be included in main.cpp
target_include_directories(${PROJECT_NAME} PUBLIC
                          "${CMAKE_SOURCE_DIR}/src/ImageProcessingUtils"
						  "${CMAKE_SOURCE_DIR}/src/ObjectDetection"
						  "${CMAKE_SOURCE_DIR}/src/GUI"
						   ${OpenCV_INCLUDE_DIRS}
                          )
					
#Link this project with te runtime output dir
target_link_directories(${PROJECT_NAME} PUBLIC
	${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

target_link_libraries(${PROJECT_NAME}
	ImageProcessingUtils
	ObjectDetection
	UserInterface
	Qt6::Core
	Qt6::Widgets
	${OpenCV_LIBS}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)
