#pragma once
#include "Detector.h"
#include "ThresholdAdjuster.h"
#include <opencv2/opencv.hpp>

class OBJECTDETECTION_API NeuralNetworkDetector : public Detector, ThresholdAdjuster {
public:
	/**
	* @brief Constructs a NeuralNetworkDetector with model and class file paths.
	* @details This constructor initializes the `NeuralNetworkDetector` by loading the specified model,
	configuration, and class names files. If loading fails, it throws a runtime_error with an error message.
	* @param[in] modelFilePath The file path to the neural network model.
	* @param[in] configFilePath The file path to the neural network configuration file (optional).
	* @param[in] classesFilePath The file path to the class names file.
	*/
	NeuralNetworkDetector(const std::string& modelFilePath, const std::string& configFilePath, const std::string& classesFilePath);

	NeuralNetworkDetector();

	/**
	 * @brief Detect objects in an input image using the loaded neural network.
	 * @details This function performs object detection on the input image using the loaded neural network.
	 It returns a DetectionMat containing information about detected objects, including their positions,
	 class names, and confidence scores.
	 * @param[in] image The input image for object detection.
	 * @return A DetectionMat containing detected objects.
	 */
	DetectionMat detect(const cv::Mat& image) override;

	/**
	* @brief Adjust the confidence threshold for object detection.
	* @details This function allows you to set a new confidence threshold for object detection.
	The threshold should be a value between 0 and 1.
	* @param[in] newThreshold The new confidence threshold to set.
	*/
	void adjustThreshold(float newThreshold) override;

	/**
	* @brief Get the current confidence threshold for object detection.
	* @return The current confidence threshold as a floating-point value.
	*/
	float getCurrentThreshold() override;

	/**
	* @brief Load class names from a file for object detection.
	* @details This function reads class names from a specified file and populates the internal list
	of class names used for object detection. Each line in the file is considered a class name.
	* @param[in] filePath The path to the file containing class names.
	If the file cannot be opened, the function throws a runtime_error with an error message.
	*/
	void loadClasses(const std::string& classesFilePath);

	/**
	* @brief Enable or disable object detection for a specific class.
	* @param[in] label The name of the class to enable or disable.
	* @param[in] enable `true` to enable detection for the class, `false` to disable it.
	*/
	void enableObject(const std::string& label, bool enable);

	/**
	* @brief Check if object detection is enabled for a specific class.
	* @param[in] name The name of the class to check.
	* @return `true` if detection is enabled for the class, `false` otherwise.
	*/
	bool isObjectEnabled(const std::string& name);

	/**
	* @brief Serialize the detector's configuration to a file.
	* @details This function saves the detector's configuration to a file for later use.
	* @param[in] filename The name of the file to save the configuration to.
	*/
	void serialize(const std::string& filename) const override;

	/**
	* @brief Serialize the detector's configuration to a file.
	* @details This function saves the detector's configuration to a file for later use.
	* @param[in] filename The name of the file to save the configuration to.
	*/
	void deserialize(const std::string& filename) override;

	/**
	* @brief Get the name of the serialization file.
	* @return The name of the file where the detector's configuration is serialized.
	*/
	std::string getSerializationFile() const override;

	/**
	* @brief Get the name of the serialization file.
	* @return The name of the file where the detector's configuration is serialized.
	*/
	std::vector<std::string> getClassNames();

protected:
	cv::dnn::Net net;
	std::vector<std::string> classNames;
	float confidenceThreshold;
	std::unordered_map<std::string, bool> objectEnabledMap;

	std::string modelFilePath;
	std::string classesFilePath;

	std::string serializationFile;

private:
	std::string configFilePath;
};
