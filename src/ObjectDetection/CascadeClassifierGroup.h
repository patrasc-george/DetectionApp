#pragma once
#include "CascadeClassifierDetector.h"
#include "CanToggleObjects.h"

#ifdef OBJECTDETECTION_EXPORTS
#define OBJECTDETECTION_API __declspec(dllexport)
#else
#define OBJECTDETECTION_API __declspec(dllimport)
#endif

class OBJECTDETECTION_API CascadeClassifierGroup : public Detector, CanToggleObjects {
public:
	CascadeClassifierGroup();

	void addDetector(const std::string& cascadeFilePath, const std::string& objectLabel);
	DetectionMat detect(const cv::Mat& image) override;

	void setPrimary(const std::string& label);
	std::string getPrimary() const;

	void enableObject(const std::string& objectLabel, bool enable) override;
	bool isObjectEnabled(const std::string& objectLabel) const override;

	void setObjectShape(const std::string& objectLabel, Detection::Shape);
	Detection::Shape objectShape(const std::string& objectLabel) const;

	void serialize(const std::string& filename) const override;
	void deserialize(const std::string& filename) override;

	std::string getSerializationFile() const override;
	std::vector<std::string> getObjectLabels() const override;

	CanToggleObjects* toObjectToggler() override;

	std::vector<std::string> getCascades() {
		std::vector<std::string> v;
		v.push_back(primaryDetector->getCascadeFilePath());
		for (auto& det : detectors) {
			v.push_back(det->getCascadeFilePath());
		}
		return v;
	}

private:
	std::vector<std::unique_ptr<CascadeClassifierDetector>> detectors;

	std::unordered_map<std::string, std::pair<bool, Detection::Shape>> classifierStateMap;

	std::string serializationFilePath;

	std::unique_ptr<CascadeClassifierDetector> primaryDetector;
};
