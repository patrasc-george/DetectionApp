#include "interfaces.h"

#include <vector>
#include <memory>
#include <opencv2/opencv.hpp>
    
class DetectionMat {
public:
    void addShape(std::unique_ptr<Shape> shape);
    void sortShapesByConfidence();
    void setShapeRenderStatus(size_t index, bool enableRender);
    void renderShapes(cv::Mat& image);

private:
    std::vector<std::unique_ptr<Shape>> shapes;
};
