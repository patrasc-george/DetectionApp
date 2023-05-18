project(Tests)

set(LibrarySources DetectionAppTests.cpp)
add_library(${PROJECT_NAME} SHARED ${LibrarySources})
qt_add_resources(${PROJECT_NAME} "images"
    PREFIX "/images"
    FILES test_image.png test_image2.jpg)
#Find OpenCV
if (NOT OpenCV_FOUND)
	find_package(OpenCV REQUIRED)
endif()

target_include_directories(${PROJECT_NAME} PUBLIC ${OpenCV_INCLUDE_DIRS})

#Find Qt
if (NOT Qt6Widgets_FOUND)
	find_package(Qt6Widgets REQUIRED)
endif()

target_link_libraries(${PROJECT_NAME} 
    ImageProcessingUtils
    ObjectDetection
    Qt6::Widgets 
    ${OpenCV_LIBS}
	)

#Link this project with the runtime output dir.
target_link_directories(${PROJECT_NAME} PUBLIC
	${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
	)

add_test(
    NAME ${PROJECT_NAME}
    COMMAND vstest.console.exe Tests.dll
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>
    )

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION "${CMAKE_BINARY_DIR}/DeliveryPackage/bin"
        LIBRARY DESTINATION "${CMAKE_BINARY_DIR}/DeliveryPackage/lib"
		)
