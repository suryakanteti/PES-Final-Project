/*
 * test_cbfifo.c - Implementation of the test function of cbfifo
 *
 * Author: Surya Kanteti
 *
 */

#include"test_cbfifo.h"
#include"cbfifo.h"
#include<assert.h>
#include<stdint.h>
#include<string.h>
#include<stdio.h>

#define TOTAL_TEST_CASES 4

 /*
  * Tests the functionality of the circular buffer FIFO
  *
  * Returns: Prints the number of test cases passed to the total number of
  *			test cases.
  */

void test_cbfifo()
{
	//TEST CASE 1: Write and dequeue 6 bytes

	char str[150] = "Hello";
	uint8_t output[150];

	assert(cbfifo_enqueue(TXQ, str, 6) == 6);
	assert(cbfifo_dequeue(TXQ, output, 6) == 6);
	assert(strcmp((char*)output, "Hello") == 0);

	assert(cbfifo_enqueue(RXQ, str, 6) == 6);
	assert(cbfifo_dequeue(RXQ, output, 6) == 6);
	assert(strcmp((char*)output, "Hello") == 0);

	// TEST CASE 2: Write an integer (4 bytes)

	int input = 25;

	assert(cbfifo_enqueue(RXQ, &input, sizeof(input)) == 4);
	assert(cbfifo_dequeue(RXQ, output, sizeof(int)) == 4);

	assert(*(int*)(output) == 25);

	//TEST CASE 3: Dequeue more bytes than available

	strcpy(str, "Embedded Software");

	assert(cbfifo_enqueue(RXQ, str, 17) == 17);
	assert(cbfifo_dequeue(RXQ, output, 22) == 17);

	//TEST CASE 4: Write 9 bytes, Dequeue 6 bytes and then 3 bytes

	strcpy(str, "Rockstar");
	uint8_t output1[150];

	assert(cbfifo_enqueue(TXQ, str, 9) == 9);

	assert(cbfifo_dequeue(TXQ, output1, 6) == 6);
	assert(strncmp((char*)output1, "Rockst", 6) == 0); // Compare only the first 4 characters

	assert(cbfifo_dequeue(TXQ, output1, 3) == 3);
	assert(strcmp((char*)output1, "ar") == 0);

	// TEST CASE 5: Write 0 bytes into empty buffer

	assert(cbfifo_enqueue(RXQ, str, 0) == 0);
	assert(cbfifo_dequeue(RXQ, output, 0) == 0);
	assert(cbfifo_dequeue(RXQ, output, 6) == 0); // Even in this case, it shoudl return 0.
}
