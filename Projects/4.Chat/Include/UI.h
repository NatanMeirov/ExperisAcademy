#ifndef __UI_H__
#define __UI_H__

#include <stddef.h> /* size_t */


void UIShowMenu(char* _menu);

void UIPrintMessage(char* _message);


void UIGetChoice(int *_choice);

/* 1, on success / 0, on failure , _details must to be INITIALIZED, Validate not space (' ') */
int UIGetDetail(char *_details, size_t _buffSize);

#endif /*#ifndef __UI_H__*/