#include "ImageProcessingUtils.h"
#include <qmessagebox.h>
#include <map>

using cv::Mat;

void ProcessingAlgorithms::binaryThresholding(Mat src, Mat& dst, short threshold)
{
	//cv::threshold(src, dst, threshold, 256, cv::THRESH_BINARY);
	if (src.type() != CV_8UC1)
		cv::cvtColor(src, src, cv::COLOR_BGR2GRAY);

	dst = cv::Mat(src);

	for (int y = 0; y < src.rows; y++)
		for (int x = 0; x < src.cols; x++)
			if (src.at<uchar>(y, x) < threshold)
				dst.at<uchar>(y, x) = 0;
			else
				dst.at<uchar>(y, x) = 255;

	cv::cvtColor(dst, dst, cv::COLOR_GRAY2BGR);
}

// if the value is lower than the threshold it becomes zero, otherwise it stays unchanged
void ProcessingAlgorithms::zeroThresholding(Mat src, Mat& dst, short threshold) {
	if (src.type() == CV_8UC4)
		cv::cvtColor(src, src, cv::COLOR_BGRA2BGR);
	dst = cv::Mat(src);

	for (int i = 0; i < src.rows; i++) {
		cv::Vec3b* in = src.ptr<cv::Vec3b>(i);
		for (int j = 0; j < src.cols; j++) {
			uchar* out = dst.ptr<uchar>(i, j);
			// threshold each channel individually
			for (int k = 0; k < src.channels(); k++)
				if (in[j][k] < threshold)
					out[k] = 0;
		}
	}
}

void ProcessingAlgorithms::adaptiveThresholding(Mat src, Mat& dst, short maxValue)
{
	if (src.type() != CV_8UC1)
		cv::cvtColor(src, src, cv::COLOR_BGR2GRAY);

	int blockSize = 11;
	double C = 2;
	cv::Mat sum;

	cv::integral(src, sum, CV_32S);
	dst.create(src.size(), src.type());

	for (int y = 0; y < src.rows; y++)
		for (int x = 0; x < src.cols; x++)
		{
			int x1 = std::max(0, x - blockSize / 2);
			int y1 = std::max(0, y - blockSize / 2);
			int x2 = std::min(src.cols - 1, x + blockSize / 2);
			int y2 = std::min(src.rows - 1, y + blockSize / 2);

			double area = (x2 - x1 + 1) * (y2 - y1 + 1);
			double sumRegion = sum.at<int>(y1, x1) + sum.at<int>(y2 + 1, x2 + 1) - sum.at<int>(y1, x2 + 1) - sum.at<int>(y2 + 1, x1);

			double mean = sumRegion / area;

			double threshold;
			threshold = mean * (1 - C / 100.0);

			dst.at<uchar>(y, x) = src.at<uchar>(y, x) > threshold ? maxValue : 0;
		}

	cv::cvtColor(dst, dst, cv::COLOR_GRAY2BGR);
}

void ProcessingAlgorithms::truncate(cv::Mat src, cv::Mat& dst, short threshold) {
	if (src.type() == CV_8UC4)
		cv::cvtColor(src, src, cv::COLOR_BGRA2BGR);
	dst = cv::Mat(src);

	for (int i = 0; i < src.rows; i++) {
		cv::Vec3b* in = src.ptr<cv::Vec3b>(i);
		for (int j = 0; j < src.cols; j++) {
			uchar* out = dst.ptr<uchar>(i, j);
			// threshold each channel individually
			float luminance = 0.2126 * in[j][2] + 0.7152 * in[j][1] + 0.0722 * in[j][0];
			if (luminance >= threshold) {
				out[0] = threshold; out[1] = threshold; out[2] = threshold;
			}
		}
	}
}

void BGR2HSV(const cv::Mat& src, cv::Mat& dst)
{
	dst = cv::Mat(src.size(), src.type());

	for (int y = 0; y < src.rows; y++)
		for (int x = 0; x < src.cols; x++)
		{
			double r = src.ptr<uchar>(y, x)[2] / 255.0;
			double g = src.ptr<uchar>(y, x)[1] / 255.0;
			double b = src.ptr<uchar>(y, x)[0] / 255.0;

			double cmax = std::max(r, std::max(g, b));
			double cmin = std::min(r, std::min(g, b));
			double diff = cmax - cmin;
			double h = -1;
			double s = -1;

			if (cmax == cmin)
				h = 0;
			else if (cmax == r)
				h = fmod(60 * ((g - b) / diff) + 360, 360);
			else if (cmax == g)
				h = fmod(60 * ((b - r) / diff) + 120, 360);
			else if (cmax == b)
				h = fmod(60 * ((r - g) / diff) + 240, 360);

			if (cmax == 0)
				s = 0;
			else
				s = (diff / cmax) * 100;

			double v = cmax * 100;

			dst.ptr<uchar>(y, x)[0] = static_cast<uchar>(h / 2);
			dst.ptr<uchar>(y, x)[1] = static_cast<uchar>(s * 2.55);
			dst.ptr<uchar>(y, x)[2] = static_cast<uchar>(v * 2.55);
		}
}

void HSV2BGR(cv::Mat src, cv::Mat& dst)
{
	dst = cv::Mat(src.size(), CV_8UC3);

	for (int y = 0; y < src.rows; y++)
		for (int x = 0; x < src.cols; x++)
		{
			cv::Vec3b pixel = src.at<cv::Vec3b>(y, x);

			double h = pixel[0];
			double s = pixel[1] / 255.0;
			double v = pixel[2] / 255.0;

			double saturationValue = s * v;
			double hueNormalized = h / 30.0;
			double chromaX = saturationValue * (1 - std::abs(fmod(hueNormalized, 2) - 1));
			double brightnessDifference = v - saturationValue;

			double r, g, b;

			switch (int(hueNormalized))
			{
			case 0:
				r = saturationValue;
				g = chromaX;
				b = 0;
				break;
			case 1:
				r = chromaX;
				g = saturationValue;
				b = 0;
				break;
			case 2:
				r = 0;
				g = saturationValue;
				b = chromaX;
				break;
			case 3:
				r = 0;
				g = chromaX;
				b = saturationValue;
				break;
			case 4:
				r = chromaX;
				g = 0;
				b = saturationValue;
				break;
			default:
				r = saturationValue;
				g = 0;
				b = chromaX;
				break;
			}

			dst.ptr<uchar>(y, x)[2] = static_cast<uchar>((r + brightnessDifference) * 255);
			dst.ptr<uchar>(y, x)[1] = static_cast<uchar>((g + brightnessDifference) * 255);
			dst.ptr<uchar>(y, x)[0] = static_cast<uchar>((b + brightnessDifference) * 255);
		}
}

std::map<float, float> histogram(cv::Mat src)
{
	std::map<float, float> histogram;

	for (int i = 0; i < 256; i++)
		histogram[i] = 0;

	for (int i = 0; i < src.rows; i++)
		for (int j = 0; j < src.cols; j++)
			histogram[src.at<uchar>(i, j)]++;

	return histogram;
}

std::map<float, float> CDF(const std::map<float, float>& histogram)
{
	std::map<float, float> cdf;
	float cumulativeSum = 0.0f;

	for (int i = 0; i < 256; i++)
		cdf[i] = 0.0f;

	for (const auto& pair : histogram)
	{
		cumulativeSum += pair.second;
		cdf[pair.first] = cumulativeSum;
	}

	return cdf;
}

std::map<float, float> normalizeX(const std::map<float, float>& histogram)
{
	std::map<float, float> normalized;
	std::pair<float, float> pair;

	int min = std::numeric_limits<int>::max();
	int max = std::numeric_limits<int>::min();

	for (const auto& pair : histogram)
	{
		if (pair.second < min)
			min = pair.second;
		if (pair.second > max)
			max = pair.second;
	}

	for (const auto& it : histogram)
	{
		pair.first = it.first;
		pair.second = (it.second - min) / (max - min);
		normalized.insert(pair);
	}

	return normalized;
}

void ProcessingAlgorithms::histogramEqualization(cv::Mat src, cv::Mat& dst)
{
	if (src.type() == CV_8UC1)
		cv::cvtColor(src, src, cv::COLOR_GRAY2BGR);
	if (src.type() == CV_8UC4)
		cv::cvtColor(src, src, cv::COLOR_BGRA2BGR);

	dst = cv::Mat(src.size(), src.type());

	BGR2HSV(src, dst);

	cv::Mat channelV;
	cv::extractChannel(dst, channelV, 2);

	std::map<float, float> hist = histogram(channelV);

	std::map<float, float> cdf = CDF(hist);

	std::map<float, float> normalized = normalizeX(cdf);

	for (auto& it : normalized)
		it.second = round(255.0f * it.second);

	for (int i = 0; i < channelV.rows; i++)
		for (int j = 0; j < channelV.cols; j++)
			channelV.at<uchar>(i, j) = static_cast<uchar>(normalized[channelV.at<uchar>(i, j)]);

	cv::insertChannel(channelV, dst, 2);

	HSV2BGR(dst, dst);
}

void ProcessingAlgorithms::detectEdges(Mat src, Mat& dst)
{
	if (src.type() != CV_8UC1)
		cv::cvtColor(src, src, cv::COLOR_BGR2GRAY);
	dst.create(src.size(), src.type());

	for (int i = 1; i < src.rows - 1; i++)
		for (int j = 1; j < src.cols - 1; j++)
		{
			int top = src.at<uchar>(i - 1, j);
			int bottom = src.at<uchar>(i + 1, j);
			int left = src.at<uchar>(i, j - 1);
			int right = src.at<uchar>(i, j + 1);

			int difference1 = abs(top - bottom);
			int difference2 = abs(left - right);

			int totalDifference = difference1 + difference2;


			dst.at<uchar>(i, j) = totalDifference;
		}

	cv::cvtColor(dst, dst, cv::COLOR_BGRA2BGR);
}

std::map<float, float> normalizeY(const std::map<float, float>& histogram)
{
	float maxFrequency = 0;
	for (const auto& pair : histogram)
		if (pair.second > maxFrequency)
			maxFrequency = pair.second;


	std::map<float, float> normalized;
	std::pair<float, float> aux;

	float min = histogram.begin()->first;
	float max = histogram.rbegin()->first;

	for (const auto& pair : histogram)
	{
		aux.first = (pair.first - min) / (max - min);
		aux.second = pair.second;
		normalized.insert(aux);
	}

	return normalized;
}

void ProcessingAlgorithms::triangleThresholding(cv::Mat src, cv::Mat& dst)
{
	if (src.type() != CV_8UC1)
		cv::cvtColor(src, src, cv::COLOR_BGR2GRAY);

	dst = cv::Mat(src);

	std::map<float, float> hist = histogram(src);

	std::map<float, float> normalized = normalizeX(hist);

	normalized = normalizeY(normalized);

	float max;
	for (const auto& pair : normalized)
		if (pair.second == 1)
		{
			max = pair.first;
			break;
		}

	std::map<float, float> normalizedCopy = normalized;
	float maxDistance = 0;
	float distance;
	int threshold;
	int k = 0;

	if (max < (128.0 / 255.0))
	{
		std::map<float, float>::iterator it = normalized.lower_bound(max);
		normalized.erase(normalized.begin(), it);
		normalized = normalizeY(normalized);

		for (const auto& pair : normalized)
		{
			distance = (1 - pair.first) - pair.second;
			if (distance > maxDistance)
			{
				maxDistance = distance;
				threshold = k;
			}
			k++;
		}
	}
	else
	{
		std::map<float, float>::iterator it = normalized.upper_bound(max);
		normalized.erase(it, normalized.end());
		normalized = normalizeY(normalized);

		for (const auto& pair : normalized)
		{
			distance = pair.first - pair.second;
			if (distance > maxDistance)
			{
				maxDistance = distance;
				threshold = k;
			}
			k++;
		}
	}

	threshold = hist.size() - normalized.size() + threshold;
	std::cout << threshold << std::endl;
	binaryThresholding(src, dst, threshold);
}

void ProcessingAlgorithms::applyingAlgorithms(Mat& image, FrameOptions* options, const short& value)
{
	if (options->getHistogramEqualization())
		histogramEqualization(image, image);
	if (options->getBinaryThresholdingValue())
		binaryThresholding(image, image, value);
	if (options->getAdaptiveThresholdingValue())
		adaptiveThresholding(image, image, value);
	if (options->getZeroThresholdingValue())
		zeroThresholding(image, image, value);
	if (options->getDetectEdges())
		detectEdges(image, image);
	if (options->getTruncThresholdingValue())
		truncate(image, image, value);
	if (options->getTriangleThresholding())
		triangleThresholding(image, image);
}

bool ConvertMat2QImage(const Mat& src, QImage& dest) {
	switch (src.type()) {
		// 8-bit, 4 channel
	case CV_8UC4:
		dest = QImage(src.data, src.cols, src.rows, static_cast<int>(src.step), QImage::Format_ARGB32);
		return true;

		// 8-bit, 3 channel
	case CV_8UC3:
		dest = QImage(src.data, src.cols, src.rows, static_cast<int>(src.step), QImage::Format_RGB888).rgbSwapped();
		return true;

		// 8-bit, 1 channel
	case CV_8UC1:
		dest = QImage(src.data, src.cols, src.rows, static_cast<int>(src.step), QImage::Format_Grayscale8);
		return true;
	default:
		std::cout << "ConvertMat2QImage() - Mat image type not handled in switch:" << src.type() << std::endl;
		return false;
	}
}

bool ConvertQImage2Mat(const QImage& src, Mat& dest) {
	switch (src.format()) {
		// 8-bit, 4 channel
	case QImage::Format_ARGB32:
	case QImage::Format_ARGB32_Premultiplied:
		dest = Mat(src.height(), src.width(), CV_8UC4, const_cast<uchar*>(src.bits()), static_cast<size_t>(src.bytesPerLine()));
		return true;

		// 8-bit, 3 channel
	case QImage::Format_RGB32:
		dest = Mat(src.height(), src.width(), CV_8UC4, const_cast<uchar*>(src.bits()), static_cast<size_t>(src.bytesPerLine()));

		cv::cvtColor(dest, dest, cv::COLOR_BGRA2BGR);   // drop the all-white alpha channel
		return true;

		// 8-bit, 3 channel
	case QImage::Format_RGB888:

		dest = Mat(src.height(), src.width(), CV_8UC3, const_cast<uchar*>(src.rgbSwapped().bits()), static_cast<size_t>(src.rgbSwapped().bytesPerLine())).clone();
		return true;

		// 8-bit, 1 channel
	case QImage::Format_Indexed8:
		dest = Mat(src.height(), src.width(), CV_8UC1, const_cast<uchar*>(src.bits()), static_cast<size_t>(src.bytesPerLine()));
		return true;

	default:
		std::cout << "ConvertQImage2Mat() - QImage format not handled in switch:" << src.format() << std::endl;
		return false;
	}
}

QImage putLogo(const short& width, const short& height)
{
	QImage img(width, height, QImage::Format::Format_RGB32);
	QImage logo(":/assets/camera_dark.png");
	logo = logo.scaled(100, 100, Qt::KeepAspectRatio);

	QPainter p;
	img.fill(Qt::white);
	p.begin(&img);
	p.drawImage(width / 2.0 - logo.size().width() / 2.0, height / 2.0 - logo.size().height() / 2.0, logo);
	p.drawText(0, height / 2.0 + logo.size().height() / 2.0 + 20, width, 10, Qt::AlignCenter, "Camera is turned off");
	p.end();
	return img;
}

cv::Scalar IMAGEPROCESSINGUTILS_API generateColorFromString(const std::string& str) {
	uchar b = 0;
	uchar g = 0;
	uchar r = 0;
	uchar step = 1; // count the steps, so we add to each channel sequentially: 1 - b, 2 - g, 3 - r | 4 - b, 5 - g, ... 
	for (int c = 0; c < str.length(); ++c, ++step) {
		if (step == 4)
			step = 1;
		if (step == 1)
			b += str[c];
		if (step == 2)
			g += str[c];
		if (step == 3)
			r += str[c];
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
	return cv::Scalar(b, g, r);
}
