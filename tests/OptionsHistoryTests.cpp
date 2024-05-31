#include "CppUnitTest.h"
#include "../src/ImageProcessingUtils/ImageProcessingUtils.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace DetectionAppTests
{
	TEST_CLASS(OptionsHistoryTests)
	{
	public:
		TEST_METHOD(Add_test)
		{
			OptionsHistory history;
			history.add(FLIP_HORIZONTAL, 1);
			FrameOptions* options = history.get();
			Assert::AreEqual(true, options->getFlipH());
		}

		TEST_METHOD(CanUndo_test)
		{
			OptionsHistory history;
			Assert::IsFalse(history.canUndo());
			history.add(FLIP_HORIZONTAL, 1);
			Assert::IsTrue(history.canUndo());
		}

		TEST_METHOD(CanRedo_test)
		{
			OptionsHistory history;
			Assert::IsFalse(history.canRedo());
			history.add(FLIP_HORIZONTAL, 1);
			history.undo();
			Assert::IsTrue(history.canRedo());
		}

		TEST_METHOD(UndoRedo_test)
		{
			OptionsHistory history;
			history.add(FLIP_HORIZONTAL, 1);
			FrameOptions* options = history.get();
			Assert::AreEqual(true, options->getFlipH());
			history.add(FLIP_HORIZONTAL, 0);
			options = history.get();
			Assert::AreEqual(false, options->getFlipH());

			history.undo();
			options = history.get();
			Assert::AreEqual(true, options->getFlipH());

			history.redo();
			options = history.get();
			Assert::AreEqual(false, options->getFlipH());
		}
	};
}