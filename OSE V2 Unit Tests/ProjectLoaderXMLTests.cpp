#include "stdafx.h"
#include "CppUnitTest.h"
#include "../OSE V2/Project.h"
#include "../OSE V2/ProjectLoaderXML.h"
#pragma comment(lib, "../Debug/OSE V2.lib")

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace origami_sheep_engine;

namespace OSEV2UnitTests
{
	TEST_CLASS(ProjectLoaderXMLTests)
	{
	public:

		TEST_METHOD(TestLoadProject)
		{
			// TODO: Your test code here
			ProjectLoaderXML loader;
			std::unique_ptr<Project> proj = loader.loadProject("TestProject");
			Assert::AreEqual(std::string("Test Project 1"), proj->get_project_info().name);
		}

	};
}