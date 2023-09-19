#pragma once

#ifdef OBJECTDETECTION_EXPORTS
#define OBJECTDETECTION_API __declspec(dllexport)
#else
#define OBJECTDETECTION_API __declspec(dllimport)
#endif

class OBJECTDETECTION_API ThresholdAdjuster {
public:
    virtual ~ThresholdAdjuster() = default;
    virtual void adjustThreshold(float newThreshold) = 0;
};
