#include "stdafx.h"
#include "CppUnitTest.h"
#include "../OSE V2/Scene.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace OSEV2Tests
{
	TEST_CLASS(UnitTest1)
	{
	public:
		TEST_METHOD(TestMethod1)
		{
			using namespace origami_sheep_engine;
			std::string name = "Test_Scene_1";
			std::unique_ptr<Scene> scene = std::make_unique<Scene>(name);
			//Assert::AreEqual(name, scene.get_name());
		}
	};
}
