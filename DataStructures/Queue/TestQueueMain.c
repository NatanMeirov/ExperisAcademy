#include <stdio.h>
#include <stdlib.h> /* size_t */
#include "ADTDefs.h"
#include "Queue.h"
#include "TestQueue.h"


void PrintQueue(Queue* _q);


int main(void)
{
	/* Tests: */
	
	return 0;
}

void PrintQueue(Queue* _q)
{
	size_t i, numOfItems =  GetNumberOfItemsInQueue(_q), head = GetHeadIndexOfQueue(_q);
	
	
	while(numOfItems > 0)
	{
		putchar('\n');
		putchar('[');
		
		if(numOfItems == 1)
		{
			printf("%d", GetQueue(_q)[head]);
		}
		else
		{
			printf("%d, ", GetQueue(_q)[head]);
		}
		
		putchar(']');
		putchar('\n');
		
		head = (head + 1) % GetSizeOfQueue(_q);
		numOfItems--;
	}
}


/* Check Reg create and check params */
void Test_Reg_QueueCreate();

/* Check NULL create */
void Test_Null_QueueCreate();

/* Check ZeroSize create */
void Test_ZeroSize_QueueCreate();


/* Check Reg destroy and check magic num */
void Test_Reg_QueueDestroy();

/* Check DoubleDestroy destroy */
void Test_DoubleDestroy_QueueDestroy();

/* Check NULL destroy */
void Test_Null_QueueDestroy();


/* check Reg insert and check params */
/* check Overflow insert */
/* check NULL insert */


/* check Reg remove and check params */
/* check Underflow remove */
/* check NULL remove */


/* check Reg isEmpty */
/* check Full isEmpty */
/* check Empty isEmpty */
/* check NULL isEmpty */


/* check circular behaviour of head and tail */


/* check print */



