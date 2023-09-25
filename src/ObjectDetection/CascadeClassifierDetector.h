#pragma once
#include "Detector.h"
#include "DetectionMat.h"

#include <opencv2/objdetect.hpp>

#ifdef OBJECTDETECTION_EXPORTS
#define OBJECTDETECTION_API __declspec(dllexport)
#else
#define OBJECTDETECTION_API __declspec(dllimport)
#endif

class OBJECTDETECTION_API CascadeClassifierDetector : public Detector {
public:
    CascadeClassifierDetector(const std::string& cascadeFilePath, const std::string& objectLabel);
    CascadeClassifierDetector();

    DetectionMat detect(const cv::Mat& image) override;

    std::string getObjectLabel() const;

    void serialize(const std::string& filename) const override;
    void deserialize(const std::string& filename) override;

    static void read(cv::FileNode& node, CascadeClassifierDetector& detector);

    std::string getCascadeFilePath() const;

    std::string getSerializationFile() const override;
private:
    std::string cascadeFilePath;
    cv::CascadeClassifier cascade;
    std::string objectLabel;

    std::string serializationFilePath;
};
