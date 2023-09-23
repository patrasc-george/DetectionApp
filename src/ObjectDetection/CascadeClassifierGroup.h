#pragma once
#include "CascadeClassifierDetector.h"

#ifdef OBJECTDETECTION_EXPORTS
#define OBJECTDETECTION_API __declspec(dllexport)
#else
#define OBJECTDETECTION_API __declspec(dllimport)
#endif

class OBJECTDETECTION_API CascadeClassifierGroup : public Detector {
public:
	/**
	* @brief Construct a `CascadeClassifierGroup` object.
	* @details This constructor initializes a `CascadeClassifierGroup` with no detectors.
	*/
	CascadeClassifierGroup();

	/**
	 * @brief Add a cascade classifier detector to the group.
	 * @details This function adds a cascade classifier detector to the group with the specified cascade file and object label.
	 * @param[in] cascadeFilePath The file path to the cascade classifier XML file.
	 * @param[in] objectLabel The label or class name associated with the object detected by the classifier.
	 */
	void addDetector(const std::string& cascadeFilePath, const std::string& objectLabel);

	/**
	 * @brief Detect objects in an image using the group of cascade classifiers.
	 * @details This function detects objects in the input image using all enabled cascade classifiers in the group.
	 It returns a `DetectionMat` object containing the detected objects.
	 * @param[in] image The input image for object detection.
	 * @return A `DetectionMat` object containing the detected objects.
	 */
	DetectionMat detect(const cv::Mat& image) override;

	/**
	 * @brief Enable or disable a specific cascade classifier detector.
	 * @details This function allows you to enable or disable a specific cascade classifier detector in the group.
	 * @param[in] objectLabel The object label associated with the detector to enable or disable.
	 * @param[in] enable Set to `true` to enable the detector, `false` to disable it.
	 */
	void enableDetector(const std::string& objectLabel, bool enable);

	/**
	 * @brief Check if a specific cascade classifier detector is enabled.
	 * @details This function checks whether a specific cascade classifier detector in the group is enabled.
	 * @param[in] objectLabel The object label associated with the detector to check.
	 * @return `true` if the detector is enabled, `false` otherwise.
	 */
	bool isDetectorEnabled(const std::string& objectLabel) const;

	/**
	* @brief Serialize the group of cascade classifiers to a file.
	* @details This function serializes the group of cascade classifiers, including their labels and enabled status, to a file.
	* @param[in] filename The file path where the group should be serialized.
	*/
	void serialize(const std::string& filename) const override;

	/**
	 * @brief Deserialize a group of cascade classifiers from a file.
	 * @details This function deserializes a group of cascade classifiers from a file, restoring their labels and enabled status.
	 * @param[in] filename The file path from which the group should be deserialized.
	 */
	void deserialize(const std::string& filename) override;

	/**
	 * @brief Get the file path of the serialization file.
	 * @details This function returns the file path of the serialization file used for saving and loading the group.
	 * @return The file path of the serialization file.
	 */
	std::string getSerializationFile() const override;

private:
	std::vector<CascadeClassifierDetector> detectors;
	std::unordered_map<std::string, size_t> labelToIndexMap;

	std::string serializationFilePath;

	/**
	 * @brief Read a cascade classifier group from a file node.
	 * @details This static function reads a cascade classifier group from a file node and adds detectors to the group.
	 It is used during deserialization.
	 * @param[in] node The file node containing data for the group.
	 * @param[out] group The `CascadeClassifierGroup` object to which detectors are added.
	 */
	static void read(cv::FileNode& node, CascadeClassifierGroup& detector);
};
