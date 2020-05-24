// /* By Joshua Carpeggiani 
// This file conatins Unit tests for the bst structs and functions; not part of the main 
// code or assignment*/

// #include "../../vendor/unity/unity.h"
// #include "bst.h"
// #include <string.h>

// void setUp(){}
// void tearDown(){}


// // test_bstInsert tests that the bstInsert puts data in the right place
// void test_bstInsert(void)
// {	
// 	node_t *head = newNode(newData("0"));
// 	bstInsert(head, newData("1"));
// 	bstInsert(head, newData("3"));
// 	bstInsert(head, newData("2"));

// 	TEST_ASSERT(cmp(head->data->key, "0") == EQ)
// 	TEST_ASSERT(cmp(head->gt->data->key, "1") == EQ)
// 	TEST_ASSERT(cmp(head->gt->gt->data->key, "3") == EQ)
// 	TEST_ASSERT(cmp(head->gt->gt->lt->data->key, "2") == EQ)
// 	freeTree(head);
// }

// // test_bstInsertDuplicates tests that duplicates are put as linked lists instead of different nodes
// void test_bstInsertDuplicates(void)
// {	
// 	node_t *head = newNode(newData("0"));
// 	bstInsert(head, newData("1"));
// 	bstInsert(head, newData("1"));

// 	TEST_ASSERT(cmp(head->gt->data->key, "1") == EQ)
// 	TEST_ASSERT(cmp(head->gt->data->llNext->key, "1") == EQ)
// 	freeTree(head);
// }

// // void test_searchTree(void)
// // {
// // 	node_t *head = newNode(newData("0"));

// // 	bstInsert(head, newData("1"));
// // 	bstInsert(head, newData("3"));
// // 	bstInsert(head, newData("2"));
// // 	bstInsert(head, newData("3"));
// // 	dictKey_t k;
// // 	strcpy(k, "1");
// // 	TEST_ASSERT(cmp(searchTree(head, k)->data->key, k)==0);
// // 	strcpy(k, "3");
// // 	TEST_ASSERT(cmp(searchTree(head, k)->data->key, k)==0);
// // 	strcpy(k, "9");
// // 	TEST_ASSERT(searchTree(head, k) == NULL);

// // 	freeTree(head);
// // }

// // void test_PrintKey(void)
// // {
// // 	node_t *head = newNode(newData("0"));

// // 	bstInsert(head, newData("2018-3"));
// // 	bstInsert(head, newData("2018-2"));
// // 	bstInsert(head, newData("2018-1"));
// // 	bstInsert(head, newData("2018-1"));
// // 	bstInsert(head, newData("2019-4"));
// // 	bstInsert(head, newData("2018-1"));
// // 	bstInsert(head, newData("2018-1"));
// // 	bstInsert(head, newData("2018-3"));
// // 	bstInsert(head, newData("2018-2"));
// // 	bstInsert(head, newData("2018-3"));
// // 	bstInsert(head, newData("2018-2"));


// // 	dictKey_t k = 0;
// // 	// strcpy(k, "2018-1");
// // 	// strcpy(k, "9");

// // 	freeTree(head);
// // }

// int main(void){
// 	UNITY_BEGIN();
// 	RUN_TEST(test_bstInsert);
// 	RUN_TEST(test_bstInsertDuplicates);
// 	// RUN_TEST(test_PrintKey);
// 	// RUN_TEST(test_searchTree);

// 	UNITY_END();
// 	return 0;
// }
