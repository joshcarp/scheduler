/* By Joshua Carpeggiani 
This file conatins Unit tests for the bst structs and functions; not part of the main 
code or assignment*/

#include "../../vendor/unity/unity.h"
#include "tummy.h"
#include <string.h>

void setUp(){}
void tearDown(){}

// test_foo tests that the foo == 69
void test_boo(void){
	TEST_ASSERT(boo() == 69)
}

int main(void){
	UNITY_BEGIN();
	RUN_TEST(test_boo);
	UNITY_END();
	return 0;
}
