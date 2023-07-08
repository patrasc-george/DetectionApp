#include <string>

#include "RevertableOptions.h"

class IMAGEPROCESSINGUTILS_API FrameOptions {
private:
	short confidence = 0;
	short threshold = 0;
	short binaryThresholdingValue = 0;
	short zeroThresholdingValue = 0;
	short truncThresholdingValue = 0;
	short adaptiveThresholdingValue = 0;
	bool histogramEqualization = false;
	bool detectEdges = false;
	bool showFeatures = false;
	bool flipH = true; // on camera, it's true by default
	bool flipV = false;
	bool showConfidence = false;

public:
	revertable_options lastChangedOption;

	/**
	* @brief Sets the confidence value.
	* @param[in] val The confidence value to be set.
	*/
	void setConfidence(const short& val);
	/**
	 * @brief Gets the confidence value.
	 * @return Returns the confidence value.
	 */
	short getConfidence() const;

	/**
	 * @brief Sets the threshold value.
	 * @param[in] val The threshold value to be set.
	 */
	void setThreshold(const short& val);
	/**
	 * @brief Gets the threshold value.
	 * @return Returns the threshold value.
	 */
	short getThreshold() const;

	/**
	 * @brief Sets whether to show features or not.
	 * @param[in] val The boolean value to be set.
	 */
	void setShowFeatures(const bool& val);
	/**
	 * @brief Gets whether to show features or not.
	 * @return Returns true if features are to be shown, otherwise returns false.
	 */
	bool getShowFeatures() const;

	/**
	 * @brief Sets whether to flip the image horizontally or not.
	 * @param[in] val The boolean value to be set.
	 */
	void setFlipH(const bool& val);
	/**
	 * @brief Gets whether to flip the image horizontally or not.
	 * @return Returns true if the image is to be flipped horizontally, otherwise returns false.
	 */
	bool getFlipH() const;

	/**
	 * @brief Sets whether to flip the image vertically or not.
	 * @param[in] val The boolean value to be set.
	 */
	void setFlipV(const bool& val);
	/**
	 * @brief Gets whether to flip the image vertically or not.
	 * @return Returns true if the image is to be flipped horizontally, otherwise returns false.
	 */
	bool getFlipV() const;

	/**
	 * @brief Sets whether to show confidence values or not.
	 * @param[in] val The boolean value to be set.
	 */
	void setShowConfidence(const bool& val);
	/**
	 * @brief Gets whether to show confidence values or not.
	 * @return Returns true if confidence values are to be shown, otherwise returns false.
	 */
	bool getShowConfidence() const;

	/**
	 * @brief Sets the binary thresholding value.
	 * @param[in] val The binary thresholding value to be set.
	 */
	void setBinaryThresholdingValue(const short& val);
	/**
	 * @brief Gets the binary thresholding value.
	  @return Returns the binary thresholding value.
	 */
	short getBinaryThresholdingValue() const;

	/**
	 * @brief Sets the zero thresholding value.
	 * @param[in] val The zero thresholding value to be set.
	 */
	void setZeroThresholdingValue(const short& val);
	/**
	 * @brief Gets the zero thresholding value.
	 * @return Returns the zero thresholding value.
	 */
	short getZeroThresholdingValue() const;

	/**
	 * @brief Sets the zero thresholding value.
	 * @param[in] val The zero thresholding value to be set.
	 */
	void setTruncThresholdingValue(const short& val);
	/**
	 * @brief Gets the zero thresholding value.
	 * @return Returns the zero thresholding value.
	 */
	short getTruncThresholdingValue() const;

	/**
	 * @brief Sets the adaptive thresholding value.
	 * @param[in] val The adaptive thresholding value to be set.
	 */
	void setAdaptiveThresholdingValue(const short& val);
	/**
	 * @brief Gets the adaptive thresholding value.
	 * @return Returns the adaptive thresholding value.
	 */
	short getAdaptiveThresholdingValue() const;

	/**
	 * @brief Sets whether to equalize the histogram of an image or not.
	 * @param[in] val The boolean value to be set.
	 */
	void setHistogramEqualization(const bool& val);
	/**
	 * @brief Gets whether to equalize the histogram of an image or not.
	 * @return Returns true if the histogram of an image is to be equalized, otherwise returns false.
	 */
	bool getHistogramEqualization() const;

	/**
	 * @brief Sets whether to detect edges in an image or not.
	 * @param[in] val The boolean value to be set.
	 */
	void setDetectEdges(const bool& val);
	/**
	 * @brief Gets whether to detect edges in an image or not.
	 * @return Returns true if edges are to be detected in an image, otherwise returns false.
	 */
	bool getDetectEdges() const;
};