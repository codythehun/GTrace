#include "stdafx.h"
#include "CppUnitTest.h"
#include "GSceneDefParser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

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