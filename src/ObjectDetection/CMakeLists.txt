# Set the project name
project(ObjectDetection)

add_definitions(-DOBJECTDETECTION_EXPORTS)
# Take every header and source in the directory
file(GLOB HEADER_FILES "*.h")
file(GLOB SOURCE_FILES "*.cpp")

# Add the library
add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES})

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(${PROJECT_NAME} PUBLIC OpenMP::OpenMP_CXX)
endif()


# Link against OpenCV
target_include_directories(${PROJECT_NAME} PUBLIC 
	${OpenCV_INCLUDE_DIRS}
	"${CMAKE_SOURCE_DIR}/src/ObjectDetection"
	)
target_link_libraries(${PROJECT_NAME} PUBLIC 
	${OpenCV_LIBS}
	Qt6::Widgets
	)
	

install(
  TARGETS ${PROJECT_NAME}
  DESTINATION bin
  )