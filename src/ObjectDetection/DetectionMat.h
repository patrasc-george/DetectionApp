#pragma once
#include "Detection.h"

#include <iterator>
#include <opencv2/core.hpp>
#include <set>
#include <memory>

#ifdef OBJECTDETECTION_EXPORTS
#define OBJECTDETECTION_API __declspec(dllexport)
#else
#define OBJECTDETECTION_API __declspec(dllimport)
#endif
    
class OBJECTDETECTION_API DetectionMat {
public:
    void add(std::shared_ptr<Detection>& detection);
    void add(const DetectionMat& other);

    void sortByConfidence();
    void setShapeRenderStatus(size_t index, bool enableRender);
    void setShowConfidence(bool show);
    void render(cv::Mat& image);
    std::vector<Detection> getAll() const;

    class OBJECTDETECTION_API iterator {
    public:
        iterator(std::vector<std::shared_ptr<Detection>>::iterator it);
        iterator& operator++();
        iterator operator++(int);
        Detection& operator*();
        bool operator==(const iterator&) const;
        bool operator!=(const iterator&) const;
    private:
        std::vector<std::shared_ptr<Detection>>::iterator iter;
    };

private:
    std::vector<std::shared_ptr<Detection>> detections;
public:
    DetectionMat(const std::vector<std::shared_ptr<Detection>>& detections);
    DetectionMat::iterator begin();
    DetectionMat::iterator end();
    bool empty();

    DetectionMat() = default;
};
