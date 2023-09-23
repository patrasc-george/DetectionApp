#pragma once
#include <opencv2/core.hpp>

#ifdef OBJECTDETECTION_EXPORTS
#define OBJECTDETECTION_API __declspec(dllexport)
#else
#define OBJECTDETECTION_API __declspec(dllimport)
#endif

class OBJECTDETECTION_API Shape {
public:
	virtual ~Shape() = default;
	virtual void render(cv::Mat& image) const = 0;
	virtual std::string getLabel() const = 0;
	virtual double getConfidence() const = 0;
	virtual void setRenderStatus(bool render) = 0;
	virtual bool shouldRender() const = 0;
};