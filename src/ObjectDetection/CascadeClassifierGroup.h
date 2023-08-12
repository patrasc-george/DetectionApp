#include "CascadeClassifierDetector.h"

class CascadeClassifierGroup : public Detector, Serializable {
public:
    CascadeClassifierGroup();

    void addDetector(const std::string& cascadeFilePath, const std::string& objectLabel);
    std::vector<std::unique_ptr<Shape>> detect(const cv::Mat& image) override;

    void enableDetector(const std::string& objectLabel, bool enable);
    bool isDetectorEnabled(const std::string& objectLabel) const;

    void serialize(const std::string& filename) const override;
    void deserialize(const std::string& filename) override;

private:
    std::vector<CascadeClassifierDetector> detectors;
    std::unordered_map<std::string, size_t> labelToIndexMap;

    static void read(cv::FileNode& node, CascadeClassifierGroup& detector);
};
