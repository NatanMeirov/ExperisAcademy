#include "Stack.h"
#include "BracketsChecker.h"

/* Defines: */
#define LENGTH 10
#define BLOCK_SIZE 5
#define OPEN_CURLY_BRACKET '{'
#define CLOSE_CURLY_BRACKET '}'
#define OPEN_SQUARE_BRACKET '['
#define CLOSE_SQUARE_BRACKET ']'
#define OPEN_ROUND_BRACKET '('
#define CLOSE_ROUND_BRACKET ')'


/* Helper Functions Declarations: */
int IsMatchingBrackets(Stack* _stack, const char* _exp, int _index);


/*---------------------------------------------------- Main API Functions: ---------------------------------------------------------------*/

int BracketsChecker(const char* _expression)
{
	int i, isCorrect = 0;
	Stack* stack = StackCreate(LENGTH, BLOCK_SIZE);
	
	for(i = 0; _expression[i] != '\0'; i++)
	{
		if(_expression[i] == OPEN_CURLY_BRACKET
		|| _expression[i] == OPEN_SQUARE_BRACKET
		|| _expression[i] == OPEN_ROUND_BRACKET)
		{
			StackPush(stack, _expression[i]);
		}
		else if(_expression[i] == CLOSE_CURLY_BRACKET
			 || _expression[i] == CLOSE_SQUARE_BRACKET
			 || _expression[i] == CLOSE_ROUND_BRACKET)
		{
			if(StackIsEmpty(stack))
			{
				return 0; /* false */
			}
			
			if(!(IsMatchingBrackets(stack, _expression, i)))
			{
				return 0; /* false */
			}
		}
		else /* Any other character */
		{
			continue;
		}
	}

	isCorrect = (StackIsEmpty(stack));
	
	StackDestroy(stack);
	
	return isCorrect; /* 1 (true) if stack is empty => correct expression, else 0 (false) => not correct expression */
}

/*---------------------------------------------------- End of Main API Functions: --------------------------------------------------------*/


/* Helper Functions: */
int IsMatchingBrackets(Stack* _stack, const char* _exp, int _index)
{
	int item;
	
	StackPop(_stack, &item);
	
	if((item == OPEN_CURLY_BRACKET && _exp[_index] == CLOSE_CURLY_BRACKET)
	|| (item == OPEN_SQUARE_BRACKET && _exp[_index] == CLOSE_SQUARE_BRACKET)
	|| (item == OPEN_ROUND_BRACKET && _exp[_index] == CLOSE_ROUND_BRACKET))
	{
		return 1; /* true */
	}
	else
	{
		return 0; /* false */
	}
}



