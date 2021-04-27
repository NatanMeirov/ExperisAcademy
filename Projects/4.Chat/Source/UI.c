#include <stdio.h> /* printf, scanf */
#include "../Include/UI.h"

void ShowMenu(char* _menu)
{
    printf("%s", _menu);
}

void PrintMessage(char* _message)
{
    printf("%s", _message);
}

void GetChoice(int *choice)
{
    scanf("%d", choice);
}

void GetDetail(char *_details)
{
    scanf("%s", _details);
}