/* TestQueue header file */

#ifndef __TESTQUEUE_H_
#define __TESTQUEUE_H_

#include <stdlib.h>
#include "Queue.h"


int* GetQueue(Queue* _queue);


size_t GetSizeOfQueue(Queue* _queue);


size_t GetHeadIndexOfQueue(Queue* _queue);


size_t GetTailIndexOfQueue(Queue* _queue);


size_t GetNumberOfItemsInQueue(Queue* _queue);


int GetMagicNumberOfQueue(Queue* _queue);


#endif /* #ifndef __TESTQUEUE_H_ */
