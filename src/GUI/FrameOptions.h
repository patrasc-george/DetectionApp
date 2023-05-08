#include <opencv2/opencv.hpp>
#include <vector>

class FrameOptions
{

public:
    void setDetIndex(const short& detIndex);
    short getDetIndex() const;

    void setDisplayedInfoCount(const short& displayedInfoCount);
    short getDisplayedInfoCount() const;

    void setConfidence(const short& confidence);
    short getConfidence() const;

    void setThreshold(const short& threshold);
    short getThreshold() const;

    void setCameraIsOn(const bool& cameraIsOn);
    bool getCameraIsOn() const;

    void setImageIsUpload(const bool& imageIsUpload);
    bool getImageIsUpload() const;

    void setIsGrayscale(const bool& isGrayscale);
    bool getIsGrayscale() const;

    void setToggleEyes(const bool& toggleEyes);
    bool getToggleEyes() const;

    void setShowRes(const bool& showRes);
    bool getShowRes() const;

    void setFlip(const bool& flip);
    bool getFlip() const;

    void setShowConfidence(const bool& showConfidence);
    bool getShowConfidence() const;

    void setShowFPS(const bool& showFPS);
    bool getShowFPS() const;

private:
    short detIndex;
    short displayedInfoCount;
    short confidence;
    short threshold;
    bool cameraIsOn;
    bool imageIsUpload;
    bool isGrayscale;
    bool toggleEyes;
    bool showRes;
    bool flip;
    bool showConfidence;
    bool showFPS;
};