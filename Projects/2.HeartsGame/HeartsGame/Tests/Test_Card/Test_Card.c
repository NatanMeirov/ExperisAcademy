#include <stdio.h>
#include <stdlib.h> /* size_t */
#include <string.h> /* strcmp */
#include "../../Modules/Headers/Card.h"


void Test_CreateCard(void);
void Test_DestroyCard(void);
void Test_GetCardRank(void);
void Test_GetCardSuite(void);
void Test_GetCardValue(void);
void Test_GetCardName(void);


int main(void)
{
    Test_CreateCard();
    Test_DestroyCard();
    Test_GetCardRank();
    Test_GetCardSuite();
    Test_GetCardValue();
    Test_GetCardName();

    return 0;
}


void Test_CreateCard(void)
{
    Card* card = CreateCard(KING, CLUB);
    Card* card2 = CreateCard(50, 100);
    if(card && !card2)
    {
        printf("\nTest_CreateCard - Passed");
    }
    else
    {
        printf("\nTest_CreateCard - Failed");
    }

    DestroyCard(&card);
}


void Test_DestroyCard(void)
{
    Card* card = CreateCard(ACE, CLUB);

    DestroyCard(&card);
    if(!card) /* Should NULL the pointer */
    {
        printf("\nTest_DestroyCard - Passed");
    }
    else
    {
        printf("\nTest_DestroyCard - Failed");
    }
}


void Test_GetCardRank(void)
{
    Card* card = CreateCard(FIVE, HEART);

    if(GetCardRank(card) == FIVE && GetCardRank(NULL) == WRONG_RANK)
    {
        printf("\nTest_GetCardRank - Passed");
    }
    else
    {
        printf("\nTest_GetCardRank - Failed");
    }

    DestroyCard(&card);
}


void Test_GetCardSuite(void)
{
    Card* card = CreateCard(QUEEN, HEART);

    if(GetCardSuite(card) == HEART && GetCardSuite(NULL) == WRONG_SUITE)
    {
        printf("\nTest_GetCardSuite - Passed");
    }
    else
    {
        printf("\nTest_GetCardSuite - Failed");
    }

    DestroyCard(&card);
}


void Test_GetCardValue(void)
{
    Card* card = CreateCard(JACK, DIAMOND);

    if(GetCardValue(card) == 11 && GetCardValue(NULL) == 0)
    {
        printf("\nTest_GetCardValue - Passed");
    }
    else
    {
        printf("\nTest_GetCardValue - Failed");
    }

    DestroyCard(&card);
}


void Test_GetCardName(void)
{
    Card* card = CreateCard(KING, HEART);

    printf("\n%s", GetCardName(card));

    if(strcmp(GetCardName(card), "King Heart") == 0 && GetCardName(NULL) == NULL)
    {
        printf("\nTest_GetCardName - Passed");
    }
    else
    {
        printf("\nTest_GetCardName - Failed");
    }

    DestroyCard(&card);
}