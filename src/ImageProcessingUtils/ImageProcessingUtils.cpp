#include "ImageProcessingUtils.h"

using cv::Mat;

// WORK IN PROGRESS, does not work correctly
void applyKernel(Mat src, Mat& dst, Mat kernel) {
	int rows = src.rows;
	int cols = src.cols;
	dst = Mat(rows, cols, src.type());

	// size of the border that will bw added to the image
	int size = kernel.rows / 2;
	// add a border to extend the image so we can use outside pixels
	cv::copyMakeBorder(src, src, size, size, size, size, cv::BORDER_REPLICATE);

	cv::parallel_for_(cv::Range(0, rows * cols), [&](const cv::Range& range) {
		for (int r = range.start; r < range.end; r++)
		{
			int i = r / cols;
			int j = r % cols;
			double value = 0;
			for (int k = -size; k <= size; k++)
			{
				uchar* sptr = src.ptr(i + size + k);
				for (int l = -size; l <= size; l++)
				{
					value += kernel.ptr<double>(k + size)[l + size] * sptr[j + size + l];
				}
			}
			dst.ptr(i)[j] = cv::saturate_cast<uchar>(value);
		}
	});
}

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

void ProcessingAlgorithms::adaptiveThresholding(Mat src, Mat& dst, short threshold) {
	if (src.type() != CV_8UC1)
		cv::cvtColor(src, src, cv::COLOR_BGR2GRAY);
	cv::adaptiveThreshold(src, dst, threshold, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 11, 2);
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

void ProcessingAlgorithms::histogramEqualization(Mat src, Mat& dst) {
	if (src.type() != CV_8UC1)
		cv::cvtColor(src, src, cv::COLOR_BGR2GRAY);

	// Compute the histogram
	int hist[256] = { 0 };
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			hist[src.at<uchar>(i, j)]++;
		}
	}

	// Compute the cumulative distribution function
	int cdf[256] = { 0 };
	cdf[0] = hist[0];
	for (int i = 1; i < 256; i++) {
		cdf[i] = cdf[i - 1] + hist[i];
	}

	// Normalize the cdf to [0, 255]
	int cdf_min = *std::min_element(cdf, cdf + 256);
	int cdf_max = *std::max_element(cdf, cdf + 256);
	for (int i = 0; i < 256; i++) {
		cdf[i] = round(255.0 * (cdf[i] - cdf_min) / (cdf_max - cdf_min));
	}

	// Apply the equalization
	dst = src.clone();
	for (int i = 0; i < dst.rows; i++) {
		for (int j = 0; j < dst.cols; j++) {
			dst.at<uchar>(i, j) = cdf[dst.at<uchar>(i, j)];
		}
	}

	cv::cvtColor(dst, dst, cv::COLOR_GRAY2BGR);
}

void ProcessingAlgorithms::detectEdges(Mat src, Mat& dst) {
	if (src.type() == CV_8UC4)
		cv::cvtColor(src, src, cv::COLOR_BGRA2BGR);
	cv::Laplacian(src, dst, CV_8U);
	cv::normalize(dst, dst, 0, 255, cv::NORM_MINMAX);
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
		dest =QImage(src.data, src.cols, src.rows, static_cast<int>(src.step), QImage::Format_Grayscale8);
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
		dest = Mat (src.height(), src.width(), CV_8UC4, const_cast<uchar*>(src.bits()), static_cast<size_t>(src.bytesPerLine()));

		cv::cvtColor(dest, dest, cv::COLOR_BGRA2BGR);   // drop the all-white alpha channel
		return true;

	// 8-bit, 3 channel
	case QImage::Format_RGB888:

		dest = Mat(src.height(), src.width(), CV_8UC3, const_cast<uchar*>(src.rgbSwapped().bits()), static_cast<size_t>(src.rgbSwapped().bytesPerLine())).clone();
		return true;

	// 8-bit, 1 channel
	case QImage::Format_Indexed8:
		dest = Mat (src.height(), src.width(), CV_8UC1, const_cast<uchar*>(src.bits()), static_cast<size_t>(src.bytesPerLine()));
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