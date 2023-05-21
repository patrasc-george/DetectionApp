#pragma once
#include <opencv2/opencv.hpp>
#include <Qimage>

#ifdef IMAGEPROCESSINGUTILS_EXPORTS
#define IMAGEPROCESSINGUTILS_API __declspec(dllexport)
#else
#define IMAGEPROCESSINGUTILS_API __declspec(dllimport)
#endif

enum IMAGEPROCESSINGUTILS_API revertable_options {
	CONFIDENCE,
	THRESHOLD,
	SHOW_FEATURES,
	FLIP_HORIZONTAL,
	FLIP_VERTICAL,
	SHOW_CONFIDENCE,
	BINARY_THRESHOLDING,
	ZERO_THRESHOLDING,
	ADAPTIVE_THRESHOLDING,
	HISTOGRAM_EQUALIZATION,
	DETECT_EDGES
};
class IMAGEPROCESSINGUTILS_API FrameOptions {
private:
	short confidence = 0;
	short threshold = 0;
	short binaryThresholdingValue = 0;
	short zeroThresholdingValue = 0;
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

class IMAGEPROCESSINGUTILS_API OptionsHistory {
private:
	std::deque<FrameOptions> undoStack;
	std::deque<FrameOptions> redoStack;
	FrameOptions currentStatus;
	short maxOptions = 10;
	revertable_options lastChangedOption;

public:
	/**
	 * @brief Adds a new option to the history.
	 * @details This function adds a new option to the history of options.
	 It first checks if the undo stack is full (i.e., has reached its maximum size of 10).
	 If it is, it removes the oldest option from the stack.
	 It then adds the current status to the undo stack and updates the current status with the provided option and value.
	 If there are any options in the redo stack, it clears them.
	 This function can be called whenever an option is changed by the user to keep track of its previous values and allow for undo/redo functionality.
	 * @param[in] prop The option to be added.
	 * @param[in] value The value of the option to be added.
	 */
	void add(revertable_options prop, short value);

	/**
	 * @brief Checks if redo is possible.
	 * @details This function checks if redo is possible by checking if the redo stack is not empty.
	 If it is not empty, it means that there are options that can be redone and it returns true.
	 Otherwise, it returns false to indicate that redo is not possible.
	 * @return Returns true if redo is possible, otherwise returns false.
	 */
	bool canRedo();

	/**
	 * @brief Checks if undo is possible.
	 * @details This function checks if undo is possible by checking if the undo stack is not empty.
	 If it is not empty, it means that there are options that can be undone and it returns true.
	 Otherwise, it returns false to indicate that undo is not possible.
	 * @return Returns true if undo is possible, otherwise returns false.
	 */
	bool canUndo();

	/**
	 * @brief Resets the history.
	 * @details This function resets the history by clearing both the undo and redo stacks.
	 It can be called whenever the history needs to be reset (e.g., when loading a new image).
	 */
	void reset();

	/**
	 * @brief Undoes the last change.
	 * @details This function undoes the last change by moving the current status from the undo stack to the redo stack
	 and setting the current status to the previous status stored in the undo stack.
	 It can be called whenever an undo operation is requested by the user (e.g., when they click on an undo button).
	 */
	void undo();

	/**
	 * @brief Redoes the last change.
	 * @details This function redoes the last change by moving the current status from the redo stack to the undo stack
	 and setting the current status to the previous status stored in the redo stack.
	 It can be called whenever a redo operation is requested by the user (e.g., when they click on a redo button).
	 */
	void redo();

	/**
	 * @brief Gets the current status.
	 * @details This function returns a pointer to the current status.
	 It can be called at any time to get a reference to the current status and access its properties or methods.
	 * @return Returns a pointer to the current status.
	 */
	FrameOptions* get();

	/**
	 * @brief Returns a string describing the last changed option.
	 * @details This function returns a string describing the last changed option.
	 It can be called at any time to get a human-readable description of the last option that was changed by the user.
	 * @return A string describing the last changed option.
	 */
	std::string lastChange();
};

/**
 * @brief Applies binary thresholding to an image.
 * @details This function applies binary thresholding to an image using OpenCV's threshold function.
 It sets all pixel values below the specified threshold to 0 and all pixel values above the threshold to 255.
 The resulting image is a binary image where all pixels are either black or white.
 * @param[in,out] image The image to be processed.
 * @param[in] threshold The threshold value.
 */
void IMAGEPROCESSINGUTILS_API binaryThresholding(cv::Mat& image, short threshold);

/**
 * @brief Applies zero thresholding to an image.
 * @details This function applies zero thresholding to an image using OpenCV's threshold function.
 It sets all pixel values below the specified threshold to 0 and leaves all pixel values above the threshold unchanged.
 The resulting image is a grayscale image where all pixels below the threshold are black
 and all pixels above the threshold retain their original values.
 * @param[in,out] image The image to be processed.
 * @param[in] threshold The threshold value.
 */
void IMAGEPROCESSINGUTILS_API zeroThresholding(cv::Mat& image, short threshold);

/**
 * @brief Applies adaptive thresholding to an image.
 * @details This function applies adaptive thresholding to an image using OpenCV's adaptiveThreshold function.
 It calculates a different threshold for each pixel based on its local neighborhood
 and then applies binary thresholding using that calculated threshold.
 The resulting image is a binary image where all pixels are either black or white.
 The size of the local neighborhood and the method used to calculate the thresholds can be adjusted
 by changing the parameters of the adaptiveThreshold function call.
 * @param[in,out] image The image to be processed.
 * @param[in] threshold The threshold value.
 */
void IMAGEPROCESSINGUTILS_API adaptiveThresholding(cv::Mat& image, short threshold);

/**
 * @brief Equalizes the histogram of an image.
 * @details This function equalizes the histogram of an image using OpenCV's equalizeHist function.
 It redistributes the pixel values in such a way that their histogram is approximately flat.
 This can improve the contrast of an image by stretching out its intensity range.
 The resulting image is a grayscale image with improved contrast.
 * @param[in,out] image The image to be processed.
 */
void IMAGEPROCESSINGUTILS_API histogramEqualization(cv::Mat& image);

/**
 * @brief Detects edges in an image using the Laplacian operator.
 * @details This function detects edges in an image using OpenCV's Laplacian function.
 It applies the Laplacian operator to the image, which calculates the second derivative of its intensity values.
 This highlights regions of rapid intensity change, which correspond to edges in the original image.
 The resulting edge map is then normalized and scaled to fit within the 0-255 range so that it can be displayed as a grayscale image.
 * @param[in,out] image The image to be processed.
 */
void IMAGEPROCESSINGUTILS_API detectEdges(cv::Mat& image);


class IMAGEPROCESSINGUTILS_API Timer {
private:
	std::chrono::time_point<std::chrono::steady_clock> start, end;
	std::chrono::duration<float> duration;
	int* counter;

public:
	/**
	 * @brief Constructs a Timer object and starts the timer.
	 * @details This constructor initializes a Timer object and starts its internal timer.
	 It also stores a reference to an integer counter that will be updated with the elapsed time when the Timer object is destroyed.
	 This constructor can be called at any time to start timing an operation (e.g., when processing an image).
	 * @param[out] c An integer reference to be used as a counter. (output)
	 */
	Timer(int& c);

	/**
	 * @brief Destroys the Timer object and stops the timer.
	 * @details This destructor stops the internal timer of a Timer object and calculates its duration.
	 It then updates the integer counter that was provided when constructing the Timer object with the elapsed time (in seconds).
	 This destructor is called automatically when a Timer object goes out of scope
	 or is deleted (e.g., when an image processing operation completes).
	 */
	~Timer();
};

/**
 * @brief Converts a cv::Mat to a QImage.
 * @details This function converts a cv::Mat to a QImage.
 It supports 8-bit, 4 channel; 8-bit, 3 channel; and 8-bit, 1 channel cv::Mats.
 If the cv::Mat type is not supported, the function returns false and prints an error message to the console.
 * @param[in] src The source cv::Mat to convert.
 * @param[out] dest The destination QImage to store the converted image.
 * @return Returns true if the conversion was successful, otherwise returns false.
 */
bool IMAGEPROCESSINGUTILS_API ConvertMat2QImage(const cv::Mat& src, QImage& dest);

/**
 * @brief Converts a QImage to a cv::Mat.
 * @details This function converts a QImage to a cv::Mat.
 It supports Format_ARGB32, Format_ARGB32_Premultiplied, Format_RGB32, Format_RGB888 and Format_Indexed8 QImages.
 If the QImage format is not supported, the function returns false and prints an error message to the console.
 * @param[in] src The source QImage to convert.
 * @param[out] dest The destination cv::Mat to store the converted image.
 * @return Returns true if the conversion was successful, otherwise returns false.
 */
bool IMAGEPROCESSINGUTILS_API ConvertQImage2Mat(const QImage& src, cv::Mat& dest);
