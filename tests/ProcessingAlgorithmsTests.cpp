#include "CppUnitTest.h"
#include "../src/ImageProcessingUtils/ImageProcessingUtils.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
#define TEST_IMAGE "C:/Siemens/detectionapp/tests/test_image.png"

namespace DetectionAppTests
{
	TEST_CLASS(Processing_Algorithms_Tests) {
public:

	TEST_METHOD(BinaryThreshold_test)
	{
		QImage qimg(TEST_IMAGE);
		cv::Mat mat;
		ConvertQImage2Mat(qimg, mat);
		binaryThresholding(mat, 100);
		Assert::IsFalse(mat.empty());
		Assert::IsTrue(ConvertMat2QImage(mat, qimg));
	}

	TEST_METHOD(HistogramEqualization_test)
	{
		QImage qimg(TEST_IMAGE);
		cv::Mat mat;
		ConvertQImage2Mat(qimg, mat);
		histogramEqualization(mat);
		Assert::IsFalse(mat.empty());
		Assert::IsTrue(ConvertMat2QImage(mat, qimg));
	}

	TEST_METHOD(AdaptiveThreshold_test)
	{
		QImage qimg(TEST_IMAGE);
		cv::Mat mat;
		ConvertQImage2Mat(qimg, mat);
		adaptiveThresholding(mat, 100);
		Assert::IsFalse(mat.empty());
		Assert::IsTrue(ConvertMat2QImage(mat, qimg));
	}

	TEST_METHOD(ZeroThreshold_test)
	{
		QImage qimg(TEST_IMAGE);
		cv::Mat mat;
		ConvertQImage2Mat(qimg, mat);
		zeroThresholding(mat, 100);
		Assert::IsFalse(mat.empty());
		Assert::IsTrue(ConvertMat2QImage(mat, qimg));
	}

	TEST_METHOD(EdgeDetection_test)
	{
		QImage qimg(TEST_IMAGE);
		cv::Mat mat;
		ConvertQImage2Mat(qimg, mat);
		detectEdges(mat);
		Assert::IsFalse(mat.empty());
		Assert::IsTrue(ConvertMat2QImage(mat, qimg));
	}

	};
}