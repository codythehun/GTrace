#include "stdafx.h"
#include "CppUnitTest.h"
#include "GSceneDefParser.h"
#include<string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

std::string scene_def =
"Plane floor"
"{"
"	Position: (0, -10, 0)"
"	Normal: (0, 1, 0)"
"	"
"sdfsf";

namespace Tests
{		
	TEST_CLASS(GSceneDefParserTest)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			Assert::AreEqual(parser_test(), 346);
			// TODO: Your test code here
		}

	};
}