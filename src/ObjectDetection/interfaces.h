#pragma once
#include <opencv2/opencv.hpp>
#include <memory>

#ifdef OBJECTDETECTION_EXPORTS
#define OBJECTDETECTION_API __declspec(dllexport)
#else
#define OBJECTDETECTION_API __declspec(dllimport)
#endif

class OBJECTDETECTION_API Shape {
public:
    virtual ~Shape() {}
    virtual void render(cv::Mat& image) const = 0;
    virtual std::string getLabel() const = 0;
    virtual double getConfidence() const = 0;
    virtual void setRenderStatus(bool render) = 0;
    virtual bool shouldRender() const = 0;
};

class OBJECTDETECTION_API Detector {
public:
    virtual ~Detector() {}
    virtual std::vector<std::unique_ptr<Shape>> detect(const cv::Mat& image) = 0;
};

class OBJECTDETECTION_API DetectionRenderer {
public:
    static void render(cv::Mat& image, const std::vector<std::unique_ptr<Shape>>& shapes);
};

class OBJECTDETECTION_API ThresholdAdjuster {
public:
    virtual ~ThresholdAdjuster() {}
    virtual void adjustThreshold(float newThreshold) = 0;
};

class Serializable {
public:
    virtual ~Serializable() {}
    virtual void serialize(const std::string& filePath) const = 0;
    virtual void deserialize(const std::string& filePath) = 0;
};
