#include "CppUnitTest.h"
#include "../src/ImageProcessingUtils/ImageProcessingUtils.h"
#include "../src/ObjectDetection/ObjectDetection.h"
#include <opencv2/opencv.hpp>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
#define TEST_IMAGE ":/images/test_image.png"

namespace DetectionAppTests
{

	TEST_CLASS(Qt_and_CV_image_conversion) {
	public:

		TEST_METHOD(Read_mat)
		{
			cv::Mat mat = cv::imread(TEST_IMAGE, cv::IMREAD_COLOR);
			Assert::IsFalse(mat.empty());
		}

		TEST_METHOD(Read_qimg)
		{
			const QImage qimg(TEST_IMAGE);
			Assert::IsFalse(qimg.isNull());
		}

		TEST_METHOD(ConvertMat2QImage_test)
		{
			cv::Mat matIn;
			matIn = cv::imread(TEST_IMAGE);
			QImage qimgOut;
			Assert::IsTrue(ConvertMat2QImage(matIn, qimgOut));
		}

		TEST_METHOD(ConvertQImage2Mat_test)
		{
			const QImage qimgIn(TEST_IMAGE);
			cv::Mat matOut;
			Assert::IsTrue(ConvertQImage2Mat(qimgIn, matOut));
		}

	};

	TEST_CLASS(Processing_Algorithms) {
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
