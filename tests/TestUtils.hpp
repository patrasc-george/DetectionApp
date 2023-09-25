#include <filesystem>

/**
 * @brief Retrieves the path to a test resource.
 * @details This function constructs the path to a test resource file using a relative path.
 If the input path starts with "../", it is treated as a relative path from the parent directory
 of the current working directory. Otherwise, it is assumed to be a relative path within the "tests"
 directory of the parent directory. The constructed path is returned as a string.
 * @param[in] s The relative path to the test resource.
 * @return The full path to the test resource as a string.
 */
static std::string test_resource(std::string s) {
	std::filesystem::path path = std::filesystem::current_path().parent_path().parent_path();
	if (s.substr(0, 3) == "../")
		s = s.substr(2, s.length() - 2);
	else
		path += "\\..\\tests\\";
	return std::string(path.string() + s);
}

/**
 * @brief Calculates the Root Mean Square (RMS) error between two images.
 * @details This function computes the RMS error between two input images, img_1 and img_2.
 It works for both grayscale and color images. For color images, it calculates the RMS error
 separately for each color channel (B, G, and R) and then combines them. The result is a percentage
 value representing the RMS error between the images.
 * @param[in] img_1 The first input image.
 * @param[in] img_2 The second input image for comparison.
 * @return The RMS error as a double-precision floating-point value.
 The RMS error is a measure of the dissimilarity between two images, with lower values indicating
 greater similarity.
 */
static double RMS_error(cv::Mat& img_1, cv::Mat& img_2) {

	double rms = 0; // result
	int H = img_1.rows;
	int W = img_1.cols;

	// 255 = max value for color / sqrt of number of color channels
	const double denominator = H * W * (255 / sqrt(3));

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

	//cv::parallel_for_(cv::Range(0, H), [&](const cv::Range& range) {
	for (int i = 0; i < H; ++i) {
		for (int j = 0; j < W; ++j) {

			// for grayscale images with just one channel
			if (img_1.channels() == 1) {
				double p0 = img_1.at<double>(i, j);
				double p1 = img_2.at<double>(i, j);
				double diff = cv::abs(p0 - p1);
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
			for (int k = 0; k < 3; ++k) {
				double diff = cv::abs(p0.val[k] - p1.val[k]);
				diff_sq += diff * diff;
			}
			// the numerator of the RMS value is a sum of square roots of differences:
			rms += sqrt(diff_sq);
		}
	}
	//});
	// divide by the total number of pixels to get a percentage value
	rms /= denominator;
	return rms;
}