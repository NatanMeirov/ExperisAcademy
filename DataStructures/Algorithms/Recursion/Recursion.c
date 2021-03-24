#include <stdio.h>
#include <stdlib.h> /* Used for size_t */
#include <string.h> /* Used for strlen */


void PrintArray(int* _array, size_t _size);
int RecursiveFactorial(int _num);
int RecursivePower(int _num, int _power);
int RecursiveSumArray(int* _array, int _sizeOfArray);
int SumArray(int* _array, int _sizeOfArray);
int RecursiveFindBiggestNumberInArray(int* _array, int _sizeOfArray, int _biggestNum);
int FindBiggestNumberInArray(int* _array, int _sizeOfArray);
size_t RecursiveFibonnaci(int _n);
size_t IterativeFibonnaci(int _n);
void HanoiTowers(int _n, char _from, char _to, char _via);
size_t RecursiveCalculateArrayAverage(int* _array, int _sizeOfArray, size_t _sum, size_t _originalSize);
size_t CalculateArrayAverage(int* _array, int _sizeOfArray);
void RecursiveSwapArray(int* _head, int* _tail);
void SwapArray(int* _array, size_t _sizeOfArray);
int RecursiveIsPalindromeString(char* _head, char* _tail);
int IsPalindromeString(char* _s);
int RecursiveCompairStrings(char* _s1, char* _s2);
int CompairStrings(char* _s1, char* _s2);
char RecursiveReturnLastCharFromString(char* _s);
char ReturnLastCharFromString(char* _s);
void RecursivePrintStarsTriangle(int _lines);
void RecursiveReversedPrintStarsTriangle(int _currentLine,int _lines);
void ReversedPrintStarsTriangleRecursively(int _lines);
void RecursivePrintMultiplicationTableTillNum(int _currentLine, int _num);
void PrintMultiplicationTableTillNum(int _num);


int main(void)
{
	int arr[] = {111,2,3,4,5,16,77777,8,9,1000};
	int arr2[] = {1,2,3,4,5};
	
	printf("%d\n", RecursivePower(7, 3));
	printf("%d\n", SumArray(arr, 10));
	printf("%d\n", FindBiggestNumberInArray(arr, 10));
	printf("%d\n", RecursiveFactorial(10));
	
	putchar('\n');
	
	printf("%ld\n", IterativeFibonnaci(10));
	printf("%ld\n", RecursiveFibonnaci(10));
	
	printf("%ld\n", IterativeFibonnaci(40));
	printf("%ld\n", RecursiveFibonnaci(40));
	
	printf("%ld\n", IterativeFibonnaci(60));
	/* printf("%ld\n", RecursiveFibonnaci(60)); */
	
	printf("%ld\n", CalculateArrayAverage(arr2, 5));
	
	HanoiTowers(3, 'A', 'B', 'C');	
		
	SwapArray(arr, 10);
	
	PrintArray(arr, 10);
	
	printf("%d\n", IsPalindromeString("abcdeedcba"));
	printf("%d\n", IsPalindromeString("Hello"));
	printf("%d\n", IsPalindromeString("Natan"));
	printf("%d\n", IsPalindromeString("natan"));
	
	printf("%d\n", CompairStrings("natan", "natan"));
	printf("%d\n", CompairStrings("natan", "nataN"));
	printf("%d\n", CompairStrings("natan", "Natan"));
	printf("%d\n", CompairStrings("hello!", "hello!"));
	
	printf("%c\n", ReturnLastCharFromString("Hello!"));
	printf("%c\n", ReturnLastCharFromString("Natan Meirov"));
	
	RecursivePrintStarsTriangle(5);
	ReversedPrintStarsTriangleRecursively(5);
	
	PrintMultiplicationTableTillNum(10);
	
	return 0;
}


void PrintArray(int* _array, size_t _size)
{
	int i;
	
	putchar('\n');
	putchar('[');
	
	for(i = 0; i < _size; i++)
	{
		if(i + 1 == _size)
		{
			printf("%d", _array[i]);
		}
		else
		{
			printf("%d, ", _array[i]);
		}
	}
	
	putchar(']');
	putchar('\n');
}


int RecursiveFactorial(int _num)
{
	if(_num <= 1) /* if (_num <= 2) return 2; - would be better and more efficient*/
	{
		return 1;
	}
	
	return (RecursiveFactorial(_num - 1) * _num);
}


int RecursivePower(int _num, int _power)
{
	if(_power <= 0)
	{
		return 1;
	}

	return (_num * RecursivePower(_num, _power - 1));
}


int RecursiveSumArray(int* _array, int _sizeOfArray)
{
	if(_sizeOfArray <= 0)
	{
		return 0;	
	}

	return (RecursiveSumArray(_array, _sizeOfArray - 1) + _array[_sizeOfArray - 1]);
}


int SumArray(int* _array, int _sizeOfArray)
{
	if(!_array)
	{
		return 0;
	}
	
	return RecursiveSumArray(_array, _sizeOfArray);
}


int RecursiveFindBiggestNumberInArray(int* _array, int _sizeOfArray, int _biggestNum)
{
	if(_sizeOfArray <= 0)
	{
		return _biggestNum;
	}

	if(_array[_sizeOfArray - 1] > _biggestNum)
	{
		return RecursiveFindBiggestNumberInArray(_array, _sizeOfArray - 1, _array[_sizeOfArray - 1]);
	}
	
	return RecursiveFindBiggestNumberInArray(_array, _sizeOfArray - 1, _biggestNum);
}


int FindBiggestNumberInArray(int* _array, int _sizeOfArray)
{
	if(!_array || _sizeOfArray == 0)
	{
		return 0;
	}

	return RecursiveFindBiggestNumberInArray(_array, _sizeOfArray - 1, _array[_sizeOfArray - 1]);
}


size_t RecursiveFibonnaci(int _n) /* O(2^n) */
{
	if(_n <= 0)
	{
		return 0;	
	}
		
	if(_n == 1)
	{
		return 1;	
	}
	
	return (RecursiveFibonnaci(_n - 1) + RecursiveFibonnaci(_n - 2));
}


size_t IterativeFibonnaci(int _n)
{
	size_t i, temp, a = 0, b = 1;
	
	if(_n <= 1)
	{
		return _n;
	}
	
	for(i = 1; i < _n; i++)
	{
		temp = b;
		b = a + b;
		a = temp;
	}

	return b;
}


size_t RecursiveCalculateArrayAverage(int* _array, int _sizeOfArray, size_t _sum, size_t _originalSize)
{
	
	if(_sizeOfArray <= 0)
	{
		return 0;
	}
	
	if(_sizeOfArray == 1)
	{
		return ((_sum + *_array) / _originalSize);
	}
	
	return RecursiveCalculateArrayAverage(_array + 1, _sizeOfArray - 1, (_sum + *_array), _originalSize);
}


size_t CalculateArrayAverage(int* _array, int _sizeOfArray)
{
	if(!_array)
	{
		return 0;
	}

	return RecursiveCalculateArrayAverage(_array, _sizeOfArray, 0, _sizeOfArray);
}

                            /* 'A'        'B'        'C' */
void HanoiTowers(int _n, char _from, char _to, char _via) /* O(2^n) */
{
	if(_n > 0)
	{
		HanoiTowers(_n - 1, _from, _to, _via);
		printf("Move %d from %c to %c\n", _n, _from, _to);
		HanoiTowers(_n - 1, _via, _to, _from);
	}
}


void RecursiveSwapArray(int* _head, int* _tail)
{
	int temp;	
	
	if(_head >= _tail)
	{
		return;
	}

	temp = *_head;
	*_head = *_tail;
	*_tail = temp;
	
	RecursiveSwapArray(_head + 1, _tail - 1);
}


void SwapArray(int* _array, size_t _sizeOfArray)
{
	if(!_array)
	{
		return;
	}

	RecursiveSwapArray(_array, (_array + _sizeOfArray - 1));
}


int RecursiveIsPalindromeString(char* _head, char* _tail)
{
	if(_head >= _tail)
	{
		return 1; /* true */
	}

	if(*_head != *_tail)
	{
		return 0; /* false */
	}
	
	return RecursiveIsPalindromeString(_head + 1, _tail - 1);
}


int IsPalindromeString(char* _s)
{
	if(!_s)
	{
		return -1; /* Error */
	}
	
	return RecursiveIsPalindromeString(_s, (_s + strlen(_s) - 1));
}


int RecursiveCompairStrings(char* _s1, char* _s2)
{
	if(*_s1 == '\0' && *_s2 == '\0')
	{
		return 1; /* true */
	}
	
	if(*_s1 != *_s2)
	{
		return 0; /* false */
	}
	
	return RecursiveCompairStrings(_s1 + 1, _s2 + 1);
}


int CompairStrings(char* _s1, char* _s2)
{
	if(!_s1 || !_s2)
	{
		return -1; /* Error */
	}

	return RecursiveCompairStrings(_s1, _s2);
}


char RecursiveReturnLastCharFromString(char* _s)
{
	if(*(_s + 1) == '\0')
	{
		return *_s; 
	}
	
	return RecursiveReturnLastCharFromString(_s + 1);
}


char ReturnLastCharFromString(char* _s)
{
	if(!_s)
	{
		return 0;
	}
	
	return RecursiveReturnLastCharFromString(_s);
}


void RecursivePrintStarsTriangle(int _lines)
{
	int i;
	
	if(_lines <= 0)
	{
		return;
	}
	
	for(i = 0; i < _lines; i++)
	{
		putchar('*');
	}
	putchar('\n');
	
	RecursivePrintStarsTriangle(_lines - 1);
}


void RecursiveReversedPrintStarsTriangle(int _currentLine, int _lines)
{
	int i;
	
	if(_currentLine > _lines)
	{
		return;
	}
	
	for(i = 0; i < _currentLine; i++)
	{
		putchar('*');
	}
	putchar('\n');
	
	RecursiveReversedPrintStarsTriangle(_currentLine + 1, _lines);
}


void ReversedPrintStarsTriangleRecursively(int _lines)
{
	RecursiveReversedPrintStarsTriangle(0, _lines);
}


void RecursivePrintMultiplicationTableTillNum(int _currentLine, int _num)
{
	int i;
	
	if(_currentLine > _num)
	{
		return;
	}
	
	for(i = 1; i <= _num; i++)
	{
		printf("%d ", (i * _currentLine));
	}
	putchar('\n');
	
	RecursivePrintMultiplicationTableTillNum(_currentLine + 1, _num);
}


void PrintMultiplicationTableTillNum(int _num)
{
	RecursivePrintMultiplicationTableTillNum(1, _num);
}
