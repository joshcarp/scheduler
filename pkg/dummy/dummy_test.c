/* By Joshua Carpeggiani 
This file conatins Unit tests for the bst structs and functions; not part of the main 
code or assignment*/

#include "../../vendor/unity/unity.h"
#include "dummy.h"
#include <string.h>

void setUp(){}
void tearDown(){}

// test_foo tests that the foo == 69
void test_foo(void){
	TEST_ASSERT(foo() == 69)
}

int main(void){
	UNITY_BEGIN();
	RUN_TEST(test_foo);
	UNITY_END();
	return 0;
}
