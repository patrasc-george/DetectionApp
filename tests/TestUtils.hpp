#include <filesystem>

static std::string test_resource(std::string s) {
	std::filesystem::path path = std::filesystem::current_path().parent_path().parent_path();
	if (s.substr(0, 3) == "../")
		s = s.substr(2, s.length() - 2);
	else
		path += "\\..\\tests\\";
	return std::string(path.string() + s);
}

// compare two image by computing the Root Mean Square error
static double RMS_error(cv::Mat& img_1, cv::Mat& img_2) {
	double rms = 0; // result
	int H = img_1.rows, W = img_1.cols;

	// only for images of the same size
	if (img_1.rows != img_2.rows || img_1.cols != img_2.cols)
		return 1;

	// if one image is grayscale, convert the other one to grayscale as well
	if (img_1.channels() != img_2.channels()) {
		if (img_1.type() != CV_8UC1)
			cv::cvtColor(img_1, img_1, cv::COLOR_BGR2GRAY);
		if (img_2.type() != CV_8UC1)
			cv::cvtColor(img_2, img_2, cv::COLOR_BGR2GRAY);
	}

	// parallel "for loop" from OpenMP
#pragma omp parallel for
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {

			// for grayscale images with just one channel
			if (img_1.channels() == 1) {
				double p0 = img_1.at<double>(i, j);
				double p1 = img_2.at<double>(i, j);
				double diff = abs(p0 - p1);
				rms += diff * diff;
				continue;
			}

			// get every pixel in the first image
			const cv::Vec3b p0 = img_1.at<cv::Vec3b>(i, j);
			// get every pixel in the second image
			const cv::Vec3b p1 = img_2.at<cv::Vec3b>(i, j);
			// the square of the differences of each pixel
			double diff_sq = 0;
			// iterate through each channel in each pixel and compute the square of the difference
			for (int k = 0; k < 3; k++) {
				double diff = abs(p0.val[k] - p1.val[k]);
				diff_sq += diff * diff;
			}
			// compute the numerator of the RMS value:
			// take the square root, divide by the max value (255) and by the square root of the number of channels
			rms += sqrt(diff_sq) / 255.0 / sqrt(3.0);
		}
	}
	// divide by the total number of pixels to get a percentage value
	rms /= H * W;
	return rms;
}