# Qt 6.2.4 uses CMake 3.16, so that should be our minimum CMake version
cmake_minimum_required(VERSION 3.16)

# Set the name for the project
project(DetectionApp)

# Specify where should the executables and libraries created
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Application)

find_package(OpenCV REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Gui Core Widgets)

# Add the subdirectories for the different parts of the application
add_subdirectory(tests)
add_subdirectory(src/Application)
add_subdirectory(src/ImageProcessingUtils)
add_subdirectory(src/ObjectDetection)
add_subdirectory(src/GUI)


include(InstallRequiredSystemLibraries)

#Set the install package for OpenCV
install (FILES
	$<TARGET_FILE:opencv_world> DESTINATION
	DESTINATION bin
	)

#Set the install package for QT
install (FILES 
	$<TARGET_FILE:Qt6::Widgets> 
	$<TARGET_FILE:Qt6::Gui>
	$<TARGET_FILE:Qt6::Core>
	DESTINATION bin
	)
install(DIRECTORY "${Qt6_DIR}/../../../plugins/platforms" DESTINATION bin)

#
# CPACK macros below here
#
set (CPACK_PACKAGE_NAME "DetectionApp")
set (CPACK_PACKAGE_VERSION "1.0.0")
set (CPACK_PACKAGE_VERSION_MAJOR "1")
set (CPACK_PACKAGE_VERSION_MINOR "0")
set (CPACK_PACKAGE_VERSION_PATCH "0")
set (CPACK_PACKAGE_INSTALL_DIRECTORY "Detection App - Siemens Internship 2023")

# Define NSIS installation types
set(CPACK_ALL_INSTALL_TYPES Full Developer)
set(CPACK_COMPONENT_LIBRARIES_INSTALL_TYPES Developer Full)
set(CPACK_COMPONENT_HEADERS_INSTALL_TYPES Developer Full)
set(CPACK_COMPONENT_APPLICATIONS_INSTALL_TYPES Full)
 
# Must be after the last CPACK macros
include(CPack)