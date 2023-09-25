#include <opencv2/imgproc.hpp>

#include "Detection.h"

Detection::Detection(const cv::Rect& rect, const std::string& label, double confidence)
	: rect(rect), label(label), confidence(confidence), renderEnabled(true), showConfidence(true),
	shapeColor(cv::Scalar(0, 255, 0)) {} // Default color: Green

Detection::Detection(const Detection& other) {
	rect = other.getRect();
	label = other.getLabel();
	confidence = other.getConfidence();
	renderEnabled = true;
	showConfidence = true;
	shapeColor = cv::Scalar(0, 255, 0);
}

void Detection::render(cv::Mat& image) const {
	if (shouldRender()) {
		std::string text = label;
		cv::Size label_size = cv::getTextSize(text, cv::FONT_HERSHEY_SIMPLEX, 0.7, 1.5, 0);



		switch (shape) {
		case Rectangle:
			cv::rectangle(image, rect, shapeColor, 2);
			if (showConfidence && confidence > 0)
				text = text + ": " + std::to_string(static_cast<int>(confidence * 100)) + "%";
			cv::putText(image, text, cv::Point(rect.x + 4, rect.y + label_size.height + 6)
				, cv::FONT_HERSHEY_SIMPLEX, 0.7, shapeColor, 2);
			break;
		case Circle:
			circle(image, 
				cv::Point(rect.x + rect.width / 2, rect.y + rect.height / 2), 
				cvRound((rect.width + rect.height) * 0.25), 
				cv::Scalar(239, 190, 98), 2);
			break;
		}
	}
	cv::cvtColor(image, image, cv::COLOR_BGRA2BGR);
}

std::string Detection::getLabel() const {
	return label;
}

double Detection::getConfidence() const {
	return confidence;
}

void Detection::setRenderStatus(bool render) {
	this->renderEnabled = render;
}

bool Detection::shouldRender() const {
	return renderEnabled;
}

void Detection::setConfidenceVisibility(bool visible) {
	showConfidence = visible;
}

void Detection::setColor(const cv::Scalar& color) {
	shapeColor = color;
}

cv::Rect Detection::getRect() const {
	return rect;
}

void Detection::setRect(const cv::Rect& rect) {
	this->rect = rect;
}
