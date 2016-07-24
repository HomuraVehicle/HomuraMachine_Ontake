#include "stdafx.h"
#include "CppUnitTest.h"
extern "C"{
#include "hmrTask.h"
}
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace test_hmrM2500
{
	TEST_CLASS(Test_Task){
		static int CntTask1;
		static sint16 task1(sint16 time){
			++CntTask1;
			return time;
		}
	public:
		TEST_METHOD_INITIALIZE(initializeTask){
			task_initialize();
		}
		TEST_METHOD_CLEANUP(finalizeTask){
			task_finalize();
		}
		TEST_METHOD(TaskSize){

			Assert::IsTrue(true);
		}

	};
	int TEST_CLASS(Test_Task)::CntTask1=0;
}