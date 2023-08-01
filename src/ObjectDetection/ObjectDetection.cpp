#include "ObjectDetection.h"

void drawLabel(cv::Mat& image, std::string text, cv::Rect rect, cv::Scalar color) {
	int baseLine;
	cv::Size label_size = cv::getTextSize(text, cv::FONT_HERSHEY_SIMPLEX, 0.7, 1.5, &baseLine);

	//int top = std::max(rect.y, label_size.height);
	cv::Point tlc = cv::Point(rect.x + 4, rect.y + label_size.height + 6);

	if (color == cv::Scalar()) {
		// we cut off the part with the confidence percent, if present
		size_t colon = text.rfind(": ");
		std::string className = colon != std::string::npos ? text.substr(0, colon) : text;

		uchar b = 0;
		uchar g = 0;
		uchar r = 0;
		uchar step = 1; // count the steps, so we add to each channel sequentially: 1 - b, 2 - g, 3 - r | 4 - b, 5 - g, ... 
		for (int c = 0; c < className.length(); ++c, ++step) {
			if (step == 4)
				step = 1;
			if (step == 1)
				b += className[c];
			if (step == 2)
				g += className[c];
			if (step == 3)
				r += className[c];
		}
		// we do this so the color values for each channel are not close together, resulting in black, gray or white colors
		while (abs(b - g) <= 30 || abs(b - r) <= 30 || abs(g - r) <= 30) {
			b -= 15;
			g += 45;
			r += 15;
		}

		// if a channel exceeds 255, it is clamped to 255. We apply the remainder operator so that we always get values from 0 to 255
		b %= 256;
		g %= 256;
		r %= 256;
		color = cv::Scalar(b, g, r);
	}
	putText(image, text, tlc, cv::FONT_HERSHEY_SIMPLEX, 0.7, color, 2);

	cv::rectangle(image, rect, color, 2);
}
