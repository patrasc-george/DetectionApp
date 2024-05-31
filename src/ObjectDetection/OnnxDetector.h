#pragma once
#include "NeuralNetworkDetector.h"

#include <opencv2/opencv.hpp>

class OBJECTDETECTION_API OnnxDetector : public NeuralNetworkDetector {
public:
	/**
	 * @brief Constructs an OnnxDetector with model and class file paths.
	 * @details This constructor initializes the OnnxDetector by loading the specified ONNX model
	 and class names. If loading fails, it throws a runtime_error with an error message.
	 * @param[in] modelFilePath The file path to the ONNX model.
	 * @param[in] classesFilePath The file path to the class names file.
	 */
	OnnxDetector(const std::string& modelFilePath, const std::string& classesFilePath);

	OnnxDetector();

	/**
	 * @brief Detect objects in an input image using the loaded ONNX model.
	 * @details This function performs object detection on the input image using the loaded ONNX model.
	 It returns a DetectionMat containing information about detected objects, including
	 their positions, class names, and confidence scores.
	 * @param[in] image The input image for object detection.
	 * @return A DetectionMat containing detected objects.
	 */
	DetectionMat detect(const cv::Mat& image) override;
};
