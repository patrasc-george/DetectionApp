#include "CppUnitTest.h"
#include "../src/ImageProcessingUtils/ImageProcessingUtils.h"
#include "TestUtils.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace DetectionAppTests
{
	TEST_CLASS(Processing_Algorithms_Tests) {
public:

	TEST_METHOD(BinaryThreshold_test)
	{
		cv::Mat reference = cv::imread(test_resource("binary_thresholding.png"));
		cv::Mat input = cv::imread(test_resource("test_image.jpg"));
		cv::Mat output;

		ProcessingAlgorithms::binaryThresholding(input, output, 100);
		Assert::IsTrue(RMS_error(output, reference) <= 0.05);
	}

	TEST_METHOD(HistogramEqualization_test)
	{
		cv::Mat reference = cv::imread(test_resource("histogram_equalization.png"));
		cv::Mat input = cv::imread(test_resource("test_image.jpg"));
		cv::Mat output;

		ProcessingAlgorithms::colorHistogramEqualization(input, output);
		Assert::IsTrue(RMS_error(output, reference) <= 0.05);
	}

	TEST_METHOD(AdaptiveThreshold_test)
	{
		cv::Mat reference = cv::imread(test_resource("adaptive_thresholding.png"));
		cv::Mat input = cv::imread(test_resource("test_image.jpg"));
		cv::Mat output;

		ProcessingAlgorithms::adaptiveThresholding(input, output, 100);
		Assert::IsTrue(RMS_error(output, reference) <= 0.05);
	}

	TEST_METHOD(ZeroThreshold_test)
	{
		cv::Mat reference = cv::imread(test_resource("thresholding_to_zero.png"));
		cv::Mat input = cv::imread(test_resource("test_image.jpg"));
		cv::Mat output;

		ProcessingAlgorithms::zeroThresholding(input, output, 100);
		Assert::IsTrue(RMS_error(output, reference) <= 0.05);
	}

	TEST_METHOD(Trunc_test)
	{
		cv::Mat reference = cv::imread(test_resource("truncate.png"));
		cv::Mat input = cv::imread(test_resource("test_image.jpg"));
		cv::Mat output;

		ProcessingAlgorithms::truncate(input, output, 100);
		Assert::IsTrue(RMS_error(output, reference) <= 0.05);
	}

	TEST_METHOD(EdgeDetection_test)
	{
		cv::Mat reference = cv::imread(test_resource("edge_detection.png"));
		cv::Mat input = cv::imread(test_resource("test_image.jpg"));
		cv::Mat output;

		ProcessingAlgorithms::detectEdges(input, output);
		Assert::IsTrue(RMS_error(output, reference) <= 0.05);
	}

	};
}