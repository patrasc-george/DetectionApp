#pragma once

#include "Detector.h"

#ifdef OBJECTDETECTION_EXPORTS
#define OBJECTDETECTION_API __declspec(dllexport)
#else
#define OBJECTDETECTION_API __declspec(dllimport)
#endif


class OBJECTDETECTION_API DetectorFactory {
public:
    static Detector* createDetectorFromFile(const std::string& filePath);
};
