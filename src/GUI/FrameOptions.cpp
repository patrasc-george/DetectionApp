#pragma once

#include "FrameOptions.h"

void FrameOptions::setDetIndex(const short& detIndex) {
    this->detIndex = detIndex;
}

short FrameOptions::getDetIndex() const {
    return detIndex;
}

void FrameOptions::setDisplayedInfoCount(const short& displayedInfoCount) {
    this->displayedInfoCount = displayedInfoCount;
}

short FrameOptions::getDisplayedInfoCount() const {
    return displayedInfoCount;
}

void FrameOptions::setConfidence(const short& confidence) {
    this->confidence = confidence;
}

short FrameOptions::getConfidence() const {
    return confidence;
}

void FrameOptions::setThreshold(const short& threshold) {
    this->threshold = threshold;
}

short FrameOptions::getThreshold() const {
    return threshold;
}

void FrameOptions::setCameraIsOn(const bool& cameraIsOn) {
    this->cameraIsOn = cameraIsOn;
}

bool FrameOptions::getCameraIsOn() const {
    return cameraIsOn;
}

void FrameOptions::setImageIsUpload(const bool& imageIsUpload) {
    this->imageIsUpload = imageIsUpload;
}

bool FrameOptions::getImageIsUpload() const {
    return imageIsUpload;
}

void FrameOptions::setIsGrayscale(const bool& isGrayscale) {
    this->isGrayscale = isGrayscale;
}

bool FrameOptions::getIsGrayscale() const {
    return isGrayscale;
}

void FrameOptions::setToggleEyes(const bool& toggleEyes) {
    this->toggleEyes = toggleEyes;
}

bool FrameOptions::getToggleEyes() const {
    return toggleEyes;
}

void FrameOptions::setShowRes(const bool& showRes) {
    this->showRes = showRes;
}

bool FrameOptions::getShowRes() const {
    return showRes;
}

void FrameOptions::setFlip(const bool& flip) {
    this->flip = flip;
}

bool FrameOptions::getFlip() const {
    return flip;
}

void FrameOptions::setShowConfidence(const bool& showConfidence)
{
    this->showConfidence= showConfidence;
}

bool FrameOptions::getShowConfidence() const
{
    return showConfidence;
}

void FrameOptions::setShowFPS(const bool& showFPS)
{
	this->showFPS = showFPS;
}

bool FrameOptions::getShowFPS() const
{
	return showFPS;
}
