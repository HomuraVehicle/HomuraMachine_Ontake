#include "stdafx.h"
#include "CppUnitTest.h"
#include "hmrDeviceManage.c"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace hmrM2500_Test
{		
	TEST_CLASS(test_i_value){
	public:	
		TEST_METHOD(i_value_test){
			int i=0;
			Assert::IsTrue(i==0,L"i value is error");
		}
		TEST_METHOD(i_pointer_test){
			int i=0;
			Assert::IsTrue(&i!=nullptr,L"i address is nullptr");
		}
		TEST_METHOD(i_copy_test){
			int i=0;
			int j=i;
			Assert::IsTrue(i==j,L"fail to copy");
		}

	};
}