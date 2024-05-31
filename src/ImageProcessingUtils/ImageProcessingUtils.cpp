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

void ProcessingAlgorithms::zeroThresholding(Mat src, Mat& dst, short threshold)
{
	if (src.type() != CV_8UC1)
		cv::cvtColor(src, src, cv::COLOR_BGR2GRAY);

	dst = cv::Mat(src);

	for (int y = 0; y < src.rows; y++)
		for (int x = 0; x < src.cols; x++)
			if (src.at<uchar>(y, x) < threshold)
				dst.at<uchar>(y, x) = 0;

	cv::cvtColor(dst, dst, cv::COLOR_GRAY2BGR);
}

void ProcessingAlgorithms::truncate(cv::Mat src, cv::Mat& dst, short threshold) {

	if (src.type() == CV_8UC1)
		cv::cvtColor(src, src, cv::COLOR_GRAY2BGR);
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

void BGR2HSV(const cv::Mat& src, cv::Mat& dst)
{
	dst = cv::Mat(src.size(), CV_8UC3);

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

std::map<float, float> normalizeY(const std::map<float, float>& histogram)
{
	int min = std::numeric_limits<int>::max();
	int max = std::numeric_limits<int>::min();

	for (const auto& pair : histogram)
	{
		if (pair.second < min)
			min = pair.second;
		if (pair.second > max)
			max = pair.second;
	}

	std::map<float, float> normalized;
	std::pair<float, float> aux;

	for (const auto& pair : histogram)
	{
		aux.first = pair.first;
		aux.second = (pair.second - min) / (max - min);
		normalized.insert(aux);
	}

	return normalized;
}

void ProcessingAlgorithms::grayscaleHistogramEqualization(cv::Mat src, cv::Mat& dst)
{
	if (src.type() != CV_8UC1)
		cv::cvtColor(src, src, cv::COLOR_BGR2GRAY);

	dst = cv::Mat(src.size(), src.type());

	std::map<float, float> hist = histogram(src);
	std::map<float, float> cdf = CDF(hist);
	std::map<float, float> normalized = normalizeY(cdf);

	for (auto& it : normalized)
		it.second = round(255.0f * it.second);

	for (int i = 0; i < dst.rows; i++)
		for (int j = 0; j < dst.cols; j++)
			dst.at<uchar>(i, j) = normalized[src.at<uchar>(i, j)];

	cv::cvtColor(dst, dst, cv::COLOR_GRAY2BGR);
}

void ProcessingAlgorithms::colorHistogramEqualization(cv::Mat src, cv::Mat& dst)
{
	if (src.type() == CV_8UC1)
		cv::cvtColor(src, src, cv::COLOR_GRAY2BGR);

	dst = cv::Mat(src.size(), CV_8UC3);

	BGR2HSV(src, dst);

	cv::Mat channelV;
	cv::extractChannel(dst, channelV, 2);

	std::map<float, float> hist = histogram(channelV);

	std::map<float, float> cdf = CDF(hist);

	std::map<float, float> normalized = normalizeY(cdf);

	for (auto& it : normalized)
		it.second = round(255.0f * it.second);

	for (int i = 0; i < channelV.rows; i++)
		for (int j = 0; j < channelV.cols; j++)
			channelV.at<uchar>(i, j) = static_cast<uchar>(normalized[channelV.at<uchar>(i, j)]);

	cv::insertChannel(channelV, dst, 2);

	HSV2BGR(dst, dst);
}

void ProcessingAlgorithms::triangleThresholding(cv::Mat src, cv::Mat& dst)
{
	if (src.type() != CV_8UC1)
		cv::cvtColor(src, src, cv::COLOR_BGR2GRAY);

	dst = cv::Mat(src);

	std::map<float, float> hist = histogram(src);

	std::map<float, float> normalized = normalizeY(hist);

	int max;
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

	if (max < 128)
	{
		std::map<float, float>::iterator it = normalized.lower_bound(max);
		normalized.erase(normalized.begin(), it);
	}
	else
	{
		std::map<float, float>::iterator it = normalized.upper_bound(max);
		normalized.erase(it, normalized.end());
	}

	normalized = normalizeX(normalized);

	for (const auto& pair : normalized)
	{
		if (max < 128)
			distance = (1 - pair.first) - pair.second;
		else
			distance = pair.first - pair.second;
		if (distance > maxDistance)
		{
			maxDistance = distance;
			threshold = k;
		}
		k++;
	}

	if (max < 128)
	{
		threshold = threshold + normalized.size() * 0.2;
		threshold = hist.size() - normalized.size() + threshold;
	}
	else
		threshold = threshold - normalized.size() * 0.2;

	binaryThresholding(src, dst, threshold);
}

std::vector<int> getPascalTriangle(const int& n)
{
	std::vector<int> pascalTriangle;

	std::vector<int> result;

	pascalTriangle.push_back(1);

	for (int i = 0; i < n - 1; i++)
	{
		for (int j = 0; j <= pascalTriangle.size(); j++)
		{
			if (j - 1 == -1 || j == pascalTriangle.size())
				result.push_back(1);
			else
				result.push_back(pascalTriangle[j - 1] + pascalTriangle[j]);
		}

		pascalTriangle = result;
		result.clear();
	}
	return pascalTriangle;
}

std::vector<std::vector<int>> matrixMultiplication(const std::vector<int>& pascalTriangle)
{
	std::vector<std::vector<int>> kernel;

	for (int i = 0; i < pascalTriangle.size(); i++)
	{
		std::vector<int> row;
		for (int j = 0; j < pascalTriangle.size(); j++)
			row.push_back(pascalTriangle[i] * pascalTriangle[j]);
		kernel.push_back(row);
	}

	return kernel;
}

template<typename T1, typename T2>
void applyKernel(cv::Mat src, cv::Mat& dst, std::vector<std::vector<T2>> kernel, int scaling = 1)
{
	int kernelSize = kernel.size() / 2;
	int channels = src.channels();
	T2 sum = 0;
	for (int y = kernelSize; y < src.rows - kernelSize; y++)
		for (int x = kernelSize; x < src.cols - kernelSize; x++)
			for (int c = 0; c < channels; c++)
			{
				sum = 0;
				for (int i = -kernelSize; i <= kernelSize; i++)
					for (int j = -kernelSize; j <= kernelSize; j++)
					{
						uchar srcPixel = src.at<uchar>(y + i, (x + j) * channels + c);
						sum = sum + srcPixel * kernel[i + kernelSize][j + kernelSize];
					}
				T1* dstPixel = dst.ptr<T1>(y) + x * channels + c;
				dstPixel[0] = sum / scaling;
			}
}

void ProcessingAlgorithms::binomial(cv::Mat src, cv::Mat& dst, short kernelSize)
{
	if (kernelSize % 2 == 0)
		kernelSize--;

	dst = cv::Mat(src.rows, src.cols, src.type());

	std::vector<int> pascalTriangle = getPascalTriangle(kernelSize);
	std::vector<std::vector<int>> kernel = matrixMultiplication(pascalTriangle);

	int scaling = std::pow(2, (kernelSize - 1) * 2);
	applyKernel<uchar>(src, dst, kernel, scaling);

	if (src.type() == CV_8UC1)
		cv::cvtColor(dst, dst, cv::COLOR_GRAY2BGR);
	if (src.type() == CV_8UC4)
		cv::cvtColor(dst, dst, cv::COLOR_BGRA2BGR);
}

void sobelKernel(short kernelSize, std::vector<std::vector<float>>& kernelX, std::vector<std::vector<float>>& kernelY)
{
	kernelSize = kernelSize / 2;
	for (int i = -kernelSize; i <= kernelSize; i++)
	{
		std::vector<float> rowX, rowY;
		for (int j = -kernelSize; j <= kernelSize; j++)
		{
			if (i == 0 && j == 0)
			{
				rowX.push_back(0);
				rowY.push_back(0);
				continue;
			}
			rowX.push_back((j / (std::pow(i, 2) + std::pow(j, 2))) * 2);
			rowY.push_back((i / (std::pow(i, 2) + std::pow(j, 2))) * 2);
		}
		kernelX.push_back(rowX);
		kernelY.push_back(rowY);
	}
}

void ProcessingAlgorithms::sobel(cv::Mat src, cv::Mat& dst, short kernelSize, cv::Mat* Gx, cv::Mat* Gy, cv::Mat* magnitude)
{
	if (src.type() != CV_8UC1)
		cv::cvtColor(src, src, cv::COLOR_BGR2GRAY);

	if (kernelSize % 2 == 0)
		kernelSize--;

	cv::GaussianBlur(src, src, cv::Size(kernelSize, kernelSize), 1, 1);

	cv::Mat auxMagnitude(src.size(), CV_32FC1);
	cv::Mat auxGx(src.size(), CV_32FC1);
	cv::Mat auxGy(src.size(), CV_32FC1);

	std::vector<std::vector<float>> kernelX, kernelY;
	sobelKernel(kernelSize, kernelX, kernelY);

	kernelSize = kernelSize / 2;

	applyKernel<float>(src, auxGx, kernelX);

	applyKernel<float>(src, auxGy, kernelY);

	for (int y = kernelSize; y < src.rows - kernelSize; y++)
		for (int x = kernelSize; x < src.cols - kernelSize; x++)
			auxMagnitude.at<float>(y, x) = std::sqrt(std::pow(auxGx.at<float>(y, x), 2) + std::pow(auxGy.at<float>(y, x), 2));

	if (Gx != nullptr)
		*Gx = auxGx;
	if (Gy != nullptr)
		*Gy = auxGy;
	if (magnitude != nullptr)
		*magnitude = auxMagnitude;

	cv::convertScaleAbs(auxMagnitude, dst);
	cv::cvtColor(dst, dst, cv::COLOR_GRAY2BGR);
}

void nonMaximumSuppression(cv::Mat& dst, cv::Mat magnitude, cv::Mat directions)
{
	dst = cv::Mat(magnitude.size(), CV_8UC1);

	for (int y = 2; y < magnitude.rows - 2; y++)
		for (int x = 2; x < magnitude.cols - 2; x++)
		{
			float direction = fmod(directions.at<float>(y, x), 180.0f);

			float pixel = magnitude.at<float>(y, x);
			float pixel1, pixel2, pixel3, pixel4;

			if ((direction >= 0 && direction < 22.5) || (direction >= 157.5 && direction <= 180))
			{
				pixel1 = magnitude.at<float>(y, x - 1);
				pixel2 = magnitude.at<float>(y, x - 2);
				pixel3 = magnitude.at<float>(y, x + 1);
				pixel4 = magnitude.at<float>(y, x + 2);
			}
			else if (direction >= 22.5 && direction < 67.5)
			{
				pixel1 = magnitude.at<float>(y - 1, x - 1);
				pixel2 = magnitude.at<float>(y - 2, x - 2);
				pixel3 = magnitude.at<float>(y + 1, x + 1);
				pixel4 = magnitude.at<float>(y + 2, x + 2);
			}
			else if (direction >= 67.5 && direction < 112.5)
			{
				pixel1 = magnitude.at<float>(y - 1, x);
				pixel2 = magnitude.at<float>(y - 2, x);
				pixel3 = magnitude.at<float>(y + 1, x);
				pixel4 = magnitude.at<float>(y + 2, x);
			}
			else if (direction >= 112.5 && direction <= 157.5)
			{
				pixel1 = magnitude.at<float>(y - 1, x + 1);
				pixel2 = magnitude.at<float>(y - 2, x + 2);
				pixel3 = magnitude.at<float>(y + 1, x - 1);
				pixel4 = magnitude.at<float>(y + 2, x - 2);
			}
			if (pixel < pixel1 || pixel <= pixel2 || pixel <= pixel3 || pixel <= pixel4)
				dst.at<uchar>(y, x) = 0;
			else
				dst.at<uchar>(y, x) = static_cast<int>(pixel);
		}
}

void ProcessingAlgorithms::canny(cv::Mat src, cv::Mat& dst, short threshold1, short threshold2, short kernelSize)
{
	cv::Mat Gx, Gy, GxGy, magnitude, directions;

	sobel(src, dst, kernelSize, &Gx, &Gy, &magnitude);

	cv::phase(Gx, Gy, directions, true);

	nonMaximumSuppression(dst, magnitude, directions);

	if (threshold1 > threshold2)
		std::swap(threshold1, threshold2);

	for (int y = 1; y < src.rows - 1; y++)
		for (int x = 1; x < src.cols - 1; x++)
		{
			uchar pixel = dst.at<uchar>(y, x);

			if (pixel <= threshold1)
				dst.at<uchar>(y, x) = 0;
			else if (pixel > threshold2)
				dst.at<uchar>(y, x) = 255;
			else
			{
				dst.at<uchar>(y, x) = 0;
				for (int i = -1; i <= 1; i++)
					for (int j = -1; j <= 1; j++)
						if (dst.at<uchar>(y + i, x + j) > threshold2)
							dst.at<uchar>(y, x) = 255;

			}
		}

	cv::cvtColor(dst, dst, cv::COLOR_GRAY2BGR);
}

void erosion(cv::Mat src, cv::Mat& dst, short kernelSize)
{
	dst = src.clone();

	cv::Mat srcValue;
	cv::extractChannel(src, srcValue, 2);

	cv::Mat dstValue = cv::Mat(src.size(), CV_8UC1);

	kernelSize = kernelSize / 2;
	uchar min;
	for (int y = kernelSize; y < src.rows - kernelSize; y++)
		for (int x = kernelSize; x < src.cols - kernelSize; x++)
		{
			min = 255;
			for (int i = -kernelSize; i <= kernelSize; i++)
				for (int j = -kernelSize; j <= kernelSize; j++)
				{
					if (srcValue.at<uchar>(y + i, x + j) < min)
						min = srcValue.at<uchar>(y + i, x + j);
				}
			dstValue.at<uchar>(y, x) = min;
		}

	cv::insertChannel(dstValue, dst, 2);
}

void dilation(cv::Mat src, cv::Mat& dst, short kernelSize)
{
	dst = src.clone();

	cv::Mat srcValue;
	cv::extractChannel(src, srcValue, 2);

	cv::Mat dstValue = cv::Mat(src.size(), CV_8UC1);

	kernelSize = kernelSize / 2;
	uchar max;
	for (int y = kernelSize; y < src.rows - kernelSize; y++)
		for (int x = kernelSize; x < src.cols - kernelSize; x++)
		{
			max = 0;
			for (int i = -kernelSize; i <= kernelSize; i++)
				for (int j = -kernelSize; j <= kernelSize; j++)
					if (srcValue.at<uchar>(y + i, x + j) > max)
						max = srcValue.at<uchar>(y + i, x + j);
			dstValue.at<uchar>(y, x) = max;
		}

	cv::insertChannel(dstValue, dst, 2);
}

void ProcessingAlgorithms::opening(cv::Mat src, cv::Mat& dst, short kernelSize)
{
	BGR2HSV(src, dst);

	erosion(dst, dst, kernelSize);
	dilation(dst, dst, kernelSize);

	HSV2BGR(dst, dst);
}

void ProcessingAlgorithms::applyingAlgorithms(Mat& image, FrameOptions* options, const short& value1, const short& value2, const short& kernel)
{
	if (options->getGrayscaleHistogramEqualization())
		grayscaleHistogramEqualization(image, image);
	if (options->getColorHistogramEqualization())
		colorHistogramEqualization(image, image);
	if (options->getBinaryThresholdingValue())
		binaryThresholding(image, image, value1);
	if (options->getAdaptiveThresholdingValue())
		adaptiveThresholding(image, image, value1);
	if (options->getZeroThresholdingValue())
		zeroThresholding(image, image, value1);
	if (options->getSobel())
		sobel(image, image, kernel);
	if (options->getTruncThresholdingValue())
		truncate(image, image, value1);
	if (options->getTriangleThresholding())
		triangleThresholding(image, image);
	if (options->getBinomial())
		binomial(image, image, kernel);
	if (options->getCanny())
		canny(image, image, value1, value2, kernel);
	if (options->getOpening())
		opening(image, image, kernel);
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
