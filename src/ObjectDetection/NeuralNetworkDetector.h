#include "interfaces.h"

class NeuralNetworkDetector : public Detector, ThresholdAdjuster, Serializable {
public:
    NeuralNetworkDetector(const std::string& modelFilePath, const std::string& configFilePath, const std::string& classesFilePath);
    std::vector<std::unique_ptr<Shape>> detect(const cv::Mat& image) override;
    void adjustThreshold(float newThreshold);
    void enableObject(const std::string& label, bool enable);

    void serialize(const std::string& filename) const override;
    void deserialize(const std::string& filename) override;


private:
    cv::dnn::Net net;
    std::vector<std::string> classNames;
    float confidenceThreshold;
    std::unordered_map<std::string, bool> objectEnabledMap;

    std::string modelFilePath;
    std::string configFilePath;
    std::string classesFilePath;
};
