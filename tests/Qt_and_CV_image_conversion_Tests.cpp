#include "CppUnitTest.h"
#include "../src/ImageProcessingUtils/ImageProcessingUtils.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
#define TEST_IMAGE "C:/Siemens/detectionapp/tests/test_image.png"

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
}