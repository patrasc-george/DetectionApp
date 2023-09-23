#pragma once

#include <opencv2/core.hpp>

#ifdef OBJECTDETECTION_EXPORTS
#define OBJECTDETECTION_API __declspec(dllexport)
#else
#define OBJECTDETECTION_API __declspec(dllimport)
#endif

class OBJECTDETECTION_API Detection {
public:
	/**
	* @brief Construct a `Detection` object with specified properties.
	* @details This constructor initializes a `Detection` object with the provided bounding box, label, and confidence score.
	The rendering properties are also set to their default values (enabled rendering, confidence visibility, and green color).
	* @param[in] rect The bounding box of the detected object.
	* @param[in] label The label or class name of the detected object.
	* @param[in] confidence The confidence score of the detection.
	*/
	Detection(const cv::Rect& rect, const std::string& label, double confidence);

	/**
	* @brief Render the detected object on an image.
	* @details This function renders the detected object on the specified image if rendering is enabled for the object.
	It draws a rectangle around the object's bounding box, adds the label (and confidence score if visible), and sets the color.
	* @param[in,out] image The image on which to render the detected object.
	*/
	void render(cv::Mat& image) const;

	/**
	* @brief Get the label of the detected object.
	* @details This function returns the label or class name of the detected object.
	* @return The label of the detected object.
	*/
	std::string getLabel() const;

	/**
	* @brief Get the confidence score of the detected object.
	* @details This function returns the confidence score associated with the detection.
	* @return The confidence score of the detected object.
	*/
	double getConfidence() const;

	/**
	* @brief Set the rendering status of the detected object.
	* @details This function allows you to enable or disable rendering of the detected object on an image.
	* @param[in] render Set to `true` to enable rendering, `false` to disable.
	*/
	void setRenderStatus(bool render);

	/**
	* @brief Check if the detected object should be rendered.
	* @details This function checks if rendering is enabled for the detected object.
	* @return `true` if the object should be rendered, `false` otherwise.
	*/
	bool shouldRender() const;

	/**
	* @brief Toggle the visibility of the confidence score.
	* @details This function allows you to show or hide the confidence score when rendering the detected object.
	* @param[in] visible Set to `true` to show the confidence score, `false` to hide it.
	*/
	void setConfidenceVisibility(bool visible);

	/**
	* @brief Set the color of the detected object when rendered.
	* @details This function allows you to specify the color to use when rendering the detected object.
	* @param[in] color The color to use for rendering, represented as a `cv::Scalar`.
	*/
	void setColor(const cv::Scalar& color);

	/**
	* @brief Set the color of the detected object when rendered.
	* @details This function allows you to specify the color to use when rendering the detected object.
	* @param[in] color The color to use for rendering, represented as a `cv::Scalar`.
	*/
	cv::Rect getRect();

private:
	cv::Rect rect;
	std::string label;
	double confidence;
	bool renderEnabled;
	bool showConfidence;
	cv::Scalar shapeColor;
};