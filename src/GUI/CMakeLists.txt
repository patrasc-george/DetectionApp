cmake_minimum_required(VERSION 3.0)
project(UserInterface)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC OFF)

add_definitions(-DUI_EXPORTS)

# Take every header and source in the directory
file(GLOB HEADER_FILES "*.h")
file(GLOB SOURCE_FILES "*.cpp")

# Add the library
add_library(${PROJECT_NAME} STATIC ${HEADER_FILES} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} 
	ImageProcessingUtils
	ObjectDetection
	Qt6::Widgets 
	Qt6::Core
	${OpenCV_LIBS}
	)

target_include_directories(${PROJECT_NAME} PUBLIC
	"${CMAKE_SOURCE_DIR}/src/ImageProcessingUtils"
	"${CMAKE_SOURCE_DIR}/src/ObjectDetection"
	${OpenCV_INCLUDE_DIRS}
	)
 
add_dependencies(${PROJECT_NAME}
	ImageProcessingUtils
	ObjectDetection
	)

#Link this project with the runtime output dir. Algo and Utils are also linked to this 
target_link_directories(${PROJECT_NAME} PUBLIC
	${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)
