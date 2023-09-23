#pragma once
#include "Detector.h"
#include "DetectionMat.h"

#include <opencv2/objdetect.hpp>

#ifdef OBJECTDETECTION_EXPORTS
#define OBJECTDETECTION_API __declspec(dllexport)
#else
#define OBJECTDETECTION_API __declspec(dllimport)
#endif

class OBJECTDETECTION_API CascadeClassifierDetector : public Detector {
public:
	/**
	* @brief Construct a `CascadeClassifierDetector` object with a specific cascade file and object label.
	* @details This constructor initializes a `CascadeClassifierDetector` with a specific cascade classifier file
	and the associated object label.
	* @param[in] cascadeFilePath The file path to the cascade classifier XML file.
	* @param[in] objectLabel The label or class name associated with the object detected by the classifier.
	*/
	CascadeClassifierDetector(const std::string& cascadeFilePath, const std::string& objectLabel);

	/**
	* @brief Default constructor for a `CascadeClassifierDetector` object.
	* @details This constructor initializes a `CascadeClassifierDetector` with default values and disables it.
	*/
	CascadeClassifierDetector();

	/**
	* @brief Detect objects in an image using the cascade classifier.
	* @details This function detects objects in the input image using the loaded cascade classifier.
	It returns a `DetectionMat` object containing the detected objects.
	* @param[in] image The input image for object detection.
	* @return A `DetectionMat` object containing the detected objects.
	*/
	DetectionMat detect(const cv::Mat& image) override;

	/**
	* @brief Enable or disable the cascade classifier detector.
	* @details This function allows you to enable or disable the cascade classifier detector.
	* @param[in] enable Set to `true` to enable the detector, `false` to disable it.
	*/
	void setEnabled(bool enable);

	/**
	 * @brief Check if the cascade classifier detector is enabled.
	 * @details This function checks whether the cascade classifier detector is currently enabled.
	 * @return `true` if the detector is enabled, `false` otherwise.
	 */
	bool isEnabled() const;

	/**
	 * @brief Get the object label associated with the detector.
	 * @details This function returns the object label associated with the cascade classifier detector.
	 * @return The object label.
	 */
	std::string getObjectLabel() const;

	/**
	 * @brief Serialize the cascade classifier detector to a file.
	 * @details This function serializes the cascade classifier detector, including its label and cascade file path, to a file.
	 * @param[in] filename The file path where the detector should be serialized.
	 */
	void serialize(const std::string& filename) const override;


	/**
	 * @brief Deserialize the cascade classifier detector from a file.
	 * @details This function deserializes the cascade classifier detector from a file, restoring its label and cascade file path.
	 * @param[in] filename The file path from which the detector should be deserialized.
	 */
	void deserialize(const std::string& filename) override;

	/**
	 * @brief Read a cascade classifier detector from a file node.
	 * @details This static function reads a cascade classifier detector from a file node and initializes a `CascadeClassifierDetector`.
	 It is used during deserialization.
	 * @param[in] node The file node containing data for the detector.
	 * @param[out] detector The `CascadeClassifierDetector` object to initialize.
	 */
	static void read(cv::FileNode& node, CascadeClassifierDetector& detector);

	/**
	 * @brief Get the file path of the cascade classifier XML file.
	 * @details This function returns the file path of the cascade classifier XML file used by the detector.
	 * @return The file path of the cascade classifier XML file.
	 */
	std::string getCascadeFilePath() const;


	/**
	 * @brief Get the file path of the serialization file.
	 * @details This function returns the file path of the serialization file used for saving and loading the detector.
	 * @return The file path of the serialization file.
	 */
	std::string getSerializationFile() const override;
private:
	std::string cascadeFilePath;
	cv::CascadeClassifier cascade;
	std::string objectLabel;
	bool enabled;

	std::string serializationFilePath;
};
