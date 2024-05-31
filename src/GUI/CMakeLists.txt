cmake_minimum_required(VERSION 3.0)
project(UserInterface)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC OFF)

add_definitions(-DUI_EXPORTS)

# Take every header and source in the directory
set(GUI_ROOT_DIR "${ROOT_DIR}/src/GUI")
file(
    GLOB_RECURSE GUI_FILES
    LIST_DIRECTORIES false
    "${GUI_ROOT_DIR}/*.c*"
    "${GUI_ROOT_DIR}/*.h*"
)

# Add the library
add_library(${PROJECT_NAME} STATIC ${GUI_FILES})

foreach(_source IN ITEMS ${GUI_FILES})
    get_filename_component(_source_path "${_source}" PATH)
    file(RELATIVE_PATH _source_path_rel "${GUI_ROOT_DIR}" "${_source_path}")
    string(REPLACE "/" "\\" _group_path "${_source_path_rel}")
    source_group("${_group_path}" FILES "${_source}")
endforeach()


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