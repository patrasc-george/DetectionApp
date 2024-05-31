#include "CppUnitTest.h"
#include "../src/ImageProcessingUtils/ImageProcessingUtils.h"
#include "TestUtils.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace DetectionAppTests
{

	TEST_CLASS(Qt_and_CV_image_conversion) {
public:

	TEST_METHOD(ConvertMat2QImage_test)
	{
		cv::Mat matIn;
		matIn = cv::imread(test_resource("test_image.jpg"));
		QImage qimgOut;
		Assert::IsTrue(ConvertMat2QImage(matIn, qimgOut));
	}

	TEST_METHOD(ConvertQImage2Mat_test)
	{
		const QImage qimgIn(test_resource("test_image.jpg").c_str());
		cv::Mat matOut;
		Assert::IsTrue(ConvertQImage2Mat(qimgIn, matOut));
	}

	};
}