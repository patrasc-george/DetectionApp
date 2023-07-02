#include "CppUnitTest.h"
#include "../src/ObjectDetection/ObjectDetection.h"
#include "TestUtils.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace DetectionAppTests
{
	TEST_CLASS(DetectorTests)
	{
	public:
		TEST_METHOD(FaceDetector_Init_test)
		{
			detectorProperties props;
			props.modelPath = test_resource("../src/data/haarcascades/haarcascade_frontalface_default.xml");
			FaceDetector detector(props, test_resource("../src/data/haarcascades/haarcascade_eye.xml"), test_resource("../src/data/haarcascades/haarcascade_smile.xml"));
			int result = detector.init();
			Assert::AreEqual(1, result);
		}

		TEST_METHOD(Detect_test)
		{
			detectorProperties props;
			props.modelPath = test_resource("../src/data/haarcascades/haarcascade_frontalface_default.xml");
			FaceDetector detector(props, test_resource("../src/data/haarcascades/haarcascade_eye.xml"), test_resource("../src/data/haarcascades/haarcascade_smile.xml"));
			detector.init();
			cv::Mat image = cv::imread(test_resource("test_face.jpg"));
			detector.detect(image, true);
			cv::Rect faceRect = detector.getLastRect();
			Assert::IsTrue(faceRect.width > 0 && faceRect.height > 0);
		}

		TEST_METHOD(ObjectDetector_Init_test)
		{
			detectorProperties props;
			props.infGraphPath = test_resource("../src/data/models/mobilenet_v2/ssd_mobilenet_v2_coco_2018_03_29.pbtxt.txt");
			props.modelPath = test_resource("../src/data/models/mobilenet_v2/frozen_inference_graph.pb");
			props.classNamesPath = test_resource("../src/data/models/mobilenet_v2/object_detection_classes_coco.txt");
			ObjectDetector detector(props);
			int result = detector.init();
			Assert::AreEqual(1, result);
		}
	};
}