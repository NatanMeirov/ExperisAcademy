#include "stdlib.h" /* Used for malloc */
#include "string.h" /* Used for strlen */
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
#define PLUS '+'
#define MINUS '-'
#define DIVISION '/'
#define MULTIPLICATION '*'


/* Helper Functions Declarations: */
int IsDigit(char _char);
int IsOperator(char _char);
int IsBrackets(char _char);
int IsOpenBracket(char _char);
int IsCloseBracket(char _char);
int TriggerOperation(int firstNumber, int secondNumber, char _opperation);
int IsCorrectMathematicExpression(const char* _expression);
int HasOnlyDigitsAndOperators(const char* _expression);
int IsHigherPriority(char _firstOperator, char _secondOperator);
char* ConvertInfixExpressionToPostfix(const char* _expression);
int CalculatePostfixExpression(const char* _postfixExpression);


/*---------------------------------------------------- Main API Functions: ---------------------------------------------------------------*/

int MathematicExpressionCalculator(const char* _expression)
{
	int isCorrectExpression;
	char* postfixExpression = NULL;
	
	/* Validations: */
	
	isCorrectExpression = BracketsChecker(_expression);
	
	if(!isCorrectExpression)
	{
		return -1;
	}
	
	isCorrectExpression = HasOnlyDigitsAndOperators(_expression);
	
	if(!isCorrectExpression)
	{
		return -1;
	}
	
	isCorrectExpression = IsCorrectMathematicExpression(_expression);
	
	if(!isCorrectExpression)
	{
		return -1;
	}
	
	/* Main Process: */
	
	postfixExpression = ConvertInfixExpressionToPostfix(_expression);
	
	return CalculatePostfixExpression(postfixExpression);
}

/*---------------------------------------------------- End of Main API Functions: --------------------------------------------------------*/


/* Helper Functions: */
int IsDigit(char _char)
{
	return ('0' <= _char && _char <= '9') ? 1 : 0;
}


int IsOperator(char _char)
{
	return (_char == PLUS || _char == MINUS || _char == DIVISION || _char == MULTIPLICATION) ? 1 : 0;
}


int IsBrackets(char _char)
{
	return (_char == OPEN_CURLY_BRACKET || _char == CLOSE_CURLY_BRACKET 
		 || _char == OPEN_SQUARE_BRACKET || _char == CLOSE_SQUARE_BRACKET 
		 || _char == OPEN_ROUND_BRACKET || _char == CLOSE_ROUND_BRACKET)
		 ? 1 : 0;
}


int IsOpenBracket(char _char)
{
	return (_char == OPEN_CURLY_BRACKET || _char == OPEN_SQUARE_BRACKET || _char == OPEN_ROUND_BRACKET) ? 1 : 0;
}


int IsCloseBracket(char _char)
{
	return (_char == CLOSE_CURLY_BRACKET || _char == CLOSE_SQUARE_BRACKET || _char == CLOSE_ROUND_BRACKET) ? 1 : 0;
}


int HasOnlyDigitsAndOperators(const char* _expression)
{
	int i;
	
	for(i = 0; _expression[i] != '\0'; i++)
	{
		if(!(IsOperator(_expression[i])) && !(IsDigit(_expression[i])) && !(IsBrackets(_expression[i])))
		{
			return 0; /* false */
		}
	}
	
	return 1; /* true */
}


int IsCorrectMathematicExpression(const char* _expression)
{
	int i, countOperators = 0, countDigits = 0;
	
	for(i = 0; _expression[i] != '\0'; i++)
	{
		if(IsDigit(_expression[i]))
		{
			countDigits++;
		}
		else if(IsOperator(_expression[i]))
		{
			countOperators++;
		}
		else /* IsBracket(_expression[i]) */
		{
			continue;
		}
	}
	
	return (countDigits == (countOperators + 1)) ? 1 : 0;
}


int TriggerOperation(int firstNumber, int secondNumber, char _opperation)
{
	if(_opperation == PLUS)
	{
		return (firstNumber + secondNumber);
	}
	else if(_opperation == MINUS)
	{
		return (firstNumber - secondNumber);
	}
	else if(_opperation == DIVISION)
	{
		return (firstNumber / secondNumber);
	}
	else /* _opperation == MULTIPLICATION */
	{
		return (firstNumber * secondNumber);
	}
}


int IsHigherPriority(char _firstOperator, char _secondOperator)
{
	/* Will return 1 if the first has higher priority, else 0 if both have the same priority or if the second has higher priority */
	int firstOpPriority = 0, secondOpPriority = 0;
	
	firstOpPriority = (_firstOperator == MULTIPLICATION || _firstOperator == DIVISION) ? 1 : 0;
	
	secondOpPriority = (_secondOperator == MULTIPLICATION || _secondOperator == DIVISION) ? 1 : 0;
	
	return (firstOpPriority > secondOpPriority) ? 1 : 0;
}


char* ConvertInfixExpressionToPostfix(const char* _expression)
{
	int i, temp, j;
	char* postfixExpression = malloc((strlen(_expression) + 1) * sizeof(char));
	Stack* stack = StackCreate(LENGTH, BLOCK_SIZE);
	
	
	for(i = 0, j = 0; _expression[i] != '\0'; i++)
	{
		if(IsDigit(_expression[i]))
		{
			postfixExpression[j++] = _expression[i];
		}
		else if(IsOperator(_expression[i]))
		{
			if(StackIsEmpty(stack))
			{
				StackPush(stack, _expression[i]);
			}
			else
			{
				while(!StackIsEmpty(stack))
				{
					StackTop(stack, &temp);
					
					if(IsOpenBracket(temp) || IsHigherPriority(_expression[i], temp))
					{
						StackPush(stack, _expression[i]);
						break;
					}
					else
					{
						StackPop(stack, &temp);
						postfixExpression[j++] = temp;
						
						if(StackIsEmpty(stack))
						{
							StackPush(stack, _expression[i]);
							break;
						}
					}
				}
			}
		}
		else /* IsBracket(_expression[i]) */
		{
			if(IsOpenBracket(_expression[i]))
			{
				StackPush(stack, _expression[i]);
			}
			else /* IsCloseBracket(_expression[i]) */
			{
				while(!StackIsEmpty(stack))
				{
					StackPop(stack, &temp);
				
					if(IsOpenBracket(temp))
					{
						break; /* Poped out the open bracket */
					}
					else /* An operator */
					{
						postfixExpression[j++] = temp;				
					}
				}
			}
		}
	}
	
	while(!StackIsEmpty(stack))
	{
		StackPop(stack, &temp);
		postfixExpression[j++] = temp;
	}
	
	postfixExpression[j] = '\0';
	
	StackDestroy(stack);
	
	return postfixExpression;
}
	

int CalculatePostfixExpression(const char* _postfixExpression)
{
	int i, firstNumber, secondNumber, operationResult, totalResult;	
	Stack* stack = StackCreate(LENGTH, BLOCK_SIZE);
		
	for(i = 0; _postfixExpression[i] != '\0'; i++)
	{
		if(IsDigit(_postfixExpression[i]))
		{
			StackPush(stack, _postfixExpression[i]);
		}
		else /* IsOperator(_postfixExpression[i]) */
		{
			StackPop(stack, &secondNumber);
			StackPop(stack, &firstNumber);
			
			firstNumber -= '0';
			secondNumber -= '0';
			
			operationResult = TriggerOperation(firstNumber, secondNumber, _postfixExpression[i]);
			StackPush(stack, operationResult + '0'); /* Push as ASCII representation, calculates as an integer */
		}
	}
	
	/* Finished the calculation: */
	StackPop(stack, &totalResult);
	
	StackDestroy(stack);
	
	return (totalResult - '0') /* Convert to integer */;
}



