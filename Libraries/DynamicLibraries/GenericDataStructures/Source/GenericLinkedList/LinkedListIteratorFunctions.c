/**
 * @file LinkedListIteratorFunctions.c
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief Adds additional Iterator functions to support the LinkedListIterator
 * @version 1.1
 * @date 2021-03-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */


 /* Includes: */

#include <stddef.h> /* size_t, NULL */
#include "../../Include/GenericLinkedList/LinkedListIterator.h"
#include "../../Include/GenericLinkedList/LinkedListIteratorFunctions.h"


/* Defines: */

#define MAX_SIZE_T ((size_t) -1)
#define KEEP_PROCCESSING 1
#define STOP_PROCCESSING 0
#define UNUSED(x) (void)(x)

typedef struct InnerEngineData
{
    int (*m_userFunctionToTrigger)(void* _element, void* _userContext); /* User's function */
    void* m_userFunctionToTriggerContext;
    int (*m_innerEngineAnalyzeFunction)(int _userFunctionResult, void* _innerEngineContext); /* Engine's function */
    void* m_innerEngineAnalyzeFunctionContext;
} InnerEngineData;


/* Helper Functions Declarations: */

static LinkedListIterator GenericIterationEngine(LinkedListIterator _begin, LinkedListIterator _end, InnerEngineData _innerEngineData);
static int InnerFindFirstProccessing(int _userFunctionResult, void* _notNeededContext);
static int InnerCountIfProccessing(int _userFunctionResult, void* _counter);
static int InnerForEachProccessing(int _userFunctionResult, void* _notNeededContext);


/*---------------------------------------------- Main API Functions -----------------------------------------------*/

LinkedListIterator LinkedListIteratorFindFirst(LinkedListIterator _begin, LinkedListIterator _end, PredicateFunction _predicate, void* _context)
{
    InnerEngineData iData;

    if(!_begin || !_end || !_predicate)
    {
        return NULL;
    }

    iData.m_userFunctionToTrigger = _predicate;
    iData.m_userFunctionToTriggerContext = _context;
    iData.m_innerEngineAnalyzeFunction = &InnerFindFirstProccessing;
    iData.m_innerEngineAnalyzeFunctionContext = NULL;

    return GenericIterationEngine(_begin, _end, iData);
}


size_t LinkedListIteratorCountIf(LinkedListIterator _begin, LinkedListIterator _end, PredicateFunction _predicate, void* _context)
{
    InnerEngineData iData;

    size_t counter = 0;

    if(!_begin || !_end || !_predicate)
    {
        return MAX_SIZE_T;
    }

    iData.m_userFunctionToTrigger = _predicate;
    iData.m_userFunctionToTriggerContext = _context;
    iData.m_innerEngineAnalyzeFunction = &InnerCountIfProccessing;
    iData.m_innerEngineAnalyzeFunctionContext = ((void*)&counter); /* Using a counter */

    GenericIterationEngine(_begin, _end, iData);

    return counter;
}


LinkedListIterator LinkedListIteratorForEach(LinkedListIterator _begin, LinkedListIterator _end, ListActionFunction _action, void* _context)
{
    InnerEngineData iData;

    if(!_begin || !_end || !_action)
    {
        return NULL;
    }

    iData.m_userFunctionToTrigger = _action;
    iData.m_userFunctionToTriggerContext = _context;
    iData.m_innerEngineAnalyzeFunction = &InnerForEachProccessing;
    iData.m_innerEngineAnalyzeFunctionContext = NULL;

    return GenericIterationEngine(_begin, _end, iData);
}

/*------------------------------------------- End of Main API Functions -------------------------------------------*/


/* Helper Functions: */

static LinkedListIterator GenericIterationEngine(LinkedListIterator _begin, LinkedListIterator _end, InnerEngineData _innerEngineData)
{
    LinkedListIterator iterator = _begin;
    int userFunctionResult, innerEngineAnalyze;

    while(iterator != _end)
    {
        userFunctionResult = _innerEngineData.m_userFunctionToTrigger(LinkedListIteratorGet(iterator), _innerEngineData.m_userFunctionToTriggerContext);
        innerEngineAnalyze = _innerEngineData.m_innerEngineAnalyzeFunction(userFunctionResult, _innerEngineData.m_innerEngineAnalyzeFunctionContext);

        if(innerEngineAnalyze == STOP_PROCCESSING) /* More readable */
        {
            break; /* Returning current iterator */
        }

        iterator = LinkedListIteratorNext(iterator);
    }

    return iterator; /* iterator may be _end if finished a complete iteration */
}


static int InnerFindFirstProccessing(int _userFunctionResult, void* _notNeededContext)
{
    UNUSED(_notNeededContext);

    if(_userFunctionResult)
    {
        return STOP_PROCCESSING; /* Predicate != 0 -> Found */
    }

    return KEEP_PROCCESSING;
}


static int InnerCountIfProccessing(int _userFunctionResult, void* _counter)
{
    if(_userFunctionResult)
    {
        *((size_t*)_counter) += 1;
    }

    return KEEP_PROCCESSING;
}


static int InnerForEachProccessing(int _userFunctionResult, void* _notNeededContext)
{
    UNUSED(_notNeededContext);

    if(_userFunctionResult)
    {
        return KEEP_PROCCESSING;
    }
    else /* Action returned 0 -> Stop */
    {
        return STOP_PROCCESSING;
    }
}
