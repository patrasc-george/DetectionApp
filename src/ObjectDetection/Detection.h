#pragma once

#include <opencv2/core.hpp>

#ifdef OBJECTDETECTION_EXPORTS
#define OBJECTDETECTION_API __declspec(dllexport)
#else
#define OBJECTDETECTION_API __declspec(dllimport)
#endif

class OBJECTDETECTION_API Detection {
public:
	Detection(const cv::Rect& rect, const std::string& label, double confidence);
	Detection(const Detection& other);

	void render(cv::Mat& image) const;
	std::string getLabel() const;
	double getConfidence() const;
	void setRenderStatus(bool render);
	bool shouldRender() const;
	void setConfidenceVisibility(bool visible);
	void setColor(const cv::Scalar& color);
	cv::Rect getRect() const;
	void setRect(const cv::Rect& rect);

	enum Shape { Rectangle, Circle };
	Shape shape = Rectangle;

private:
	cv::Rect rect;
	std::string label;
	double confidence;
	bool renderEnabled;
	bool showConfidence;
	cv::Scalar shapeColor;
};