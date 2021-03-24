#include <stddef.h> /* size_t */
#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc, free */
#include <string.h> /* strcpy */
#include "GenericVector.h"

#define LENGTH 10
#define STR_SIZE 20

typedef struct TypesData
{
    char** m_typeNames;
    size_t m_elmsNum;
} TypesData;

int SmartPrint(void* _item, size_t _index, void* _context);

int main(void)
{
    size_t i;
    Vector* vec = VectorCreate(LENGTH, LENGTH);
    TypesData data;
    int val1 = 777;
    char val2 = 'A';
    size_t val3 = -1;
    int* item1 = &val1;
    char* item2 = &val2;
    size_t* item3 = &val3;
    data.m_elmsNum = 3;
    data.m_typeNames = malloc(sizeof(char**) * data.m_elmsNum);
    for(i = 0; i < data.m_elmsNum; i++)
    {
        data.m_typeNames[i] = malloc(sizeof(char*) * STR_SIZE);
    }

    strcpy(data.m_typeNames[0], "int");
    strcpy(data.m_typeNames[1], "char");
    strcpy(data.m_typeNames[2], "size_t");

    /* Appending 3 diffrent types to the generic Vector */
    VectorAppend(vec, item1);
    VectorAppend(vec, item2);
    VectorAppend(vec, item3);

    VectorForEach(vec, &SmartPrint, &data);

    VectorDestroy(&vec, NULL);

    for(i = 0; i < data.m_elmsNum; i++)
    {
        free(data.m_typeNames[i]);
    }
    free(data.m_typeNames);

    return 0;
}


int SmartPrint(void* _item, size_t _index, void* _context)
{
    if(strcmp(((TypesData*)_context)->m_typeNames[_index], "int") == 0)
    {
        printf("%d\n", *((int*)_item));
    }
    else if(strcmp(((TypesData*)_context)->m_typeNames[_index], "char") == 0)
    {
        printf("%c\n", *((char*)_item));
    }
    else if(strcmp(((TypesData*)_context)->m_typeNames[_index], "size_t") == 0)
    {
        printf("%lu\n", *((size_t*)_item));
    }

    return 1; /* To continue iterating */
}
