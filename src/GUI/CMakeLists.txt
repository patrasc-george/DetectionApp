cmake_minimum_required(VERSION 3.0)
project(UserInterface)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC OFF)

add_definitions(-DUI_EXPORTS)

# Take every header and source in the directory
file(GLOB_RECURSE WIDGET_FILES "custom_widgets/*.hpp" "custom_widgets/*.h" "custom_widgets/*.cpp")
file(GLOB_RECURSE WINDOW_FILES "window/*.hpp" "window/*.h" "window/*.cpp")
file(GLOB_RECURSE SIDEMENU "sidemenu/*.hpp" "sidemenu/*.h" "sidemenu/*.cpp")

# Add the library
add_library(${PROJECT_NAME} STATIC ${WIDGET_FILES} ${WINDOW_FILES} ${SIDEMENU})

source_group("UI_Widgets" FILES ${WIDGET_FILES})
source_group("UI_Window" FILES ${WINDOW_FILES})
source_group("UI_SideMenu" FILES ${SIDEMENU})


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
	"${CMAKE_SOURCE_DIR}/src/GUI"
	${OpenCV_INCLUDE_DIRS}
	)
 
add_dependencies(${PROJECT_NAME}
	ImageProcessingUtils
	ObjectDetection
	)

target_link_directories(${PROJECT_NAME} PUBLIC
	${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)


install(
  TARGETS ${PROJECT_NAME}
  DESTINATION bin
  )