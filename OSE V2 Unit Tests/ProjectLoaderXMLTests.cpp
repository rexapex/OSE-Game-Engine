#include "stdafx.h"
#include "CppUnitTest.h"
#include "../OSE V2/OSE-Core/Game/Tag.h"
#pragma comment(lib, "../Debug/OSE V2.lib")

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ose;

namespace OSEV2UnitTests
{
	TEST_CLASS(ProjectLoaderXMLTests)
	{
	public:

		TEST_METHOD(TestLoadProject)
		{
			// TODO: Your test code here
			//Tag tag("TestTag");
			//std::unique_ptr<ProjectLoaderXML> project_loader = std::make_unique<ProjectLoaderXML>();
			//std::unique_ptr<Project> proj = project_loader->loadProject("TestProject");
			//Assert::AreEqual(std::string("Test Project 1"), proj->get_project_info().name);
		}

	};
}

