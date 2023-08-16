#pragma once
#include "CascadeClassifierDetector.h"

#ifdef OBJECTDETECTION_EXPORTS
#define OBJECTDETECTION_API __declspec(dllexport)
#else
#define OBJECTDETECTION_API __declspec(dllimport)
#endif

class OBJECTDETECTION_API CascadeClassifierGroup : public Detector {
public:
    CascadeClassifierGroup();

    void addDetector(const std::string& cascadeFilePath, const std::string& objectLabel);
    DetectionMat detect(const cv::Mat& image) override;

    void enableDetector(const std::string& objectLabel, bool enable);
    bool isDetectorEnabled(const std::string& objectLabel) const;

    void serialize(const std::string& filename) const override;
    void deserialize(const std::string& filename) override;

    std::string getSerializationFile() const override;

private:
    std::vector<CascadeClassifierDetector> detectors;
    std::unordered_map<std::string, size_t> labelToIndexMap;

    std::string serializationFilePath;

    static void read(cv::FileNode& node, CascadeClassifierGroup& detector);
};
