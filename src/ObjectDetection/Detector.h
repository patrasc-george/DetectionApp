#pragma once

#include "Serializable.h"
#include "DetectionMat.h"
#include "ThresholdAdjuster.h"
#include "CanToggleObjects.h"

#ifdef OBJECTDETECTION_EXPORTS
#define OBJECTDETECTION_API __declspec(dllexport)
#else
#define OBJECTDETECTION_API __declspec(dllimport)
#endif

class OBJECTDETECTION_API Detector : public Serializable {
public:
    virtual ~Detector() = default;
    virtual DetectionMat detect(const cv::Mat& image) = 0;

    virtual ThresholdAdjuster* toThresholdAdjuster() {
        return nullptr;
    }
    virtual CanToggleObjects* toObjectToggler() {
        return nullptr;
    }
};