#include "ImageProcessingUtils.h"
#include <qmessagebox.h>

using cv::Mat;

// if the average value of a pixel is lower than the treshold it becomes zero, otherwise it gets the max value
void ProcessingAlgorithms::binaryThresholding(Mat src, Mat& dst, short threshold) {
	if (src.type() == CV_8UC4)
		cv::cvtColor(src, src, cv::COLOR_BGRA2BGR);
	if (src.type() == CV_8UC1)
		cv::cvtColor(src, src, cv::COLOR_GRAY2BGR);
	dst = cv::Mat(src.rows, src.cols, CV_8UC3);

	for (int i = 0; i < src.rows; i++) {
		cv::Vec3b* in = src.ptr<cv::Vec3b>(i);
		for (int j = 0; j < src.cols; j++) {
			uchar* out = dst.ptr<uchar>(i, j);
			// compute the luminance of red, green and blue channels
			float luminance = 0.2126 * in[j][2] + 0.7152 * in[j][1] + 0.0722 * in[j][0];
			if (luminance >= threshold) {
				out[0] = 255; out[1] = 255; out[2] = 255;
			}
			else {
				out[0] = 0; out[1] = 0; out[2] = 0;
			}
		}
	}
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

std::vector<int> getHistogram(Mat src)
{
	std::vector<int> histogram(256, 0);
	for (int i = 0; i < src.rows; i++)
		for (int j = 0; j < src.cols; j++)
			histogram[src.at<uchar>(i, j)]++;
	return histogram;
}

std::vector<int> CDF(const std::vector<int>& histogram)
{
	std::vector<int> cdf(256, 0);
	cdf[0] = histogram[0];
	for (int i = 1; i < histogram.size(); i++)
		cdf[i] = cdf[i - 1] + histogram[i];
	return cdf;
}

std::vector<int> Normalize(std::vector<int> cdf)
{
	int cdf_min = *std::min_element(cdf.begin(), cdf.end());
	int cdf_max = *std::max_element(cdf.begin(), cdf.end());
	for (int i = 0; i < cdf.size(); i++)
		cdf[i] = round(255.0 * (cdf[i] - cdf_min) / (cdf_max - cdf_min));
	return cdf;
}

void ProcessingAlgorithms::histogramEqualization(Mat src, Mat& dst)
{
	if (src.type() != CV_8UC1)
		cv::cvtColor(src, src, cv::COLOR_BGR2GRAY);

	std::vector<int> histogram = getHistogram(src);
	std::vector<int> cdf = CDF(histogram);
	cdf = Normalize(cdf);

	dst = src.clone();
	for (int i = 0; i < dst.rows; i++)
		for (int j = 0; j < dst.cols; j++)
			dst.at<uchar>(i, j) = cdf[dst.at<uchar>(i, j)];

	cv::cvtColor(dst, dst, cv::COLOR_GRAY2BGR);
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
