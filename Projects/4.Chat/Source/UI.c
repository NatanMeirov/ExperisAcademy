#include <stdio.h> /* printf, scanf */
#include "../Include/UI.h"
#include <string.h>

#define LINE_LENGTH 200
#define SIZE_FOR_ONE_CHAR_BUFFER_AND_TERMINATE 2

void UIShowMenu(char* _menu)
{
    printf("%s", _menu);
}

void UIPrintMessage(char* _message)
{
    printf("%s", _message);
}

void UIGetChoice(int *_choice)
{
    char buffer[LINE_LENGTH];
    fgets(buffer, LINE_LENGTH, stdin);
	sscanf(buffer, "%d", _choice);
}

int UIGetDetail(char *_details, size_t _buffSize)
{
    char buffer[LINE_LENGTH];
    fgets(buffer, LINE_LENGTH, stdin);
	sscanf(buffer, "%s", _details);

    return 1;
}