#pragma once

#include "Detector.h"

#ifdef OBJECTDETECTION_EXPORTS
#define OBJECTDETECTION_API __declspec(dllexport)
#else
#define OBJECTDETECTION_API __declspec(dllimport)
#endif


class OBJECTDETECTION_API DetectorFactory {
public:
	/**
	* @brief Create a detector instance from a serialized configuration file.
	* @details This function reads a serialized configuration file at the specified path and creates
	an instance of a detector based on the information found in the file. The detector type and
	model file paths are extracted from the file and used to instantiate the appropriate detector class.
	* @param[in] filePath The path to the serialized configuration file.
	* @return A pointer to the created detector instance, or nullptr on failure.
	The function attempts to open the specified file and extract information about the detector type and
	model file paths. Depending on the detector type found in the file, it creates an instance of one
	of the following detector classes:
	CascadeClassifierDetector for "CASCADE" type
	OnnxDetector or NeuralNetworkDetector for "NETWORK" type (based on the model file extension)
	CascadeClassifierGroup for "CASCADE_NETWORK" type
	If the file cannot be opened, an exception is thrown, and nullptr is returned on failure.
	*/
	static Detector* createDetectorFromFile(const std::string& filePath);
};
