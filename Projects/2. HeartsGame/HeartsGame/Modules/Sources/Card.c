/**
 * @file Card.c
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief Card module source file
 * @version 0.1
 * @date 2021-03-09
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Includes: */

#include "../../Modules/Headers/Card.h"
#include <stdlib.h> /* malloc, free, size_t */
#include <string.h> /* strcpy, strcat */

/* Defines: */

#define MAGIC_NUM 86542973
#define STRING_SIZE 30

/* cards suites symbols */

#define RED_HEART_SYMBOL_CODE "\x1B[31m\xE2\x99\xA5\x1B[0m"
#define BLACK_SPADE_SYMBOL_CODE "\x1b[30m\xE2\x99\xA0\x1B[0m"
#define RED_DIAMOND_SYMBOL_CODE "\x1B[31m\xE2\x99\xA6\x1B[0m"
#define BLACK_CLUB_SYMBOL_CODE "\x1b[30m\xE2\x99\xA3\x1B[0m"

/* Older version codes - without colors:
#define HEART_SYMBOL_CODE "\xE2\x99\xA5"
#define SPADE_SYMBOL_CODE "\xE2\x99\xA0"
#define DIAMOND_SYMBOL_CODE "\xE2\x99\xA6"
#define CLUB_SYMBOL_CODE "\xE2\x99\xA3"
*/

struct Card
{
    Rank m_cardRank;
    Suite m_cardSuite;
    size_t m_cardValue;
    char* m_cardName;
    int m_magicNumber;
};


/* Validation Functions Declarations: */

static int ValidateCreateCardParams(Rank _rank, Suite _suite);

/* Helper Functions Declarations: */

static Card* InitializeNewCard(Card** _card, Rank _rank, Suite _suite);
static size_t ObtainCardNumericValue(Rank _rank);
static char* ObtainCardName(Rank _rank, Suite _suite);


/*-------------------------------------- Main API Functions: ---------------------------------------*/


Card* CreateCard(Rank _rank, Suite _suite)
{
    Card* newCard = NULL;

    if(!ValidateCreateCardParams(_rank, _suite))
    {
        return NULL;
    }

    newCard = malloc(sizeof(Card));
    if(!newCard)
    {
        return NULL;
    }

    return InitializeNewCard(&newCard, _rank, _suite);
}


void DestroyCard(Card** _card)
{
    if(_card && *_card && (*_card)->m_magicNumber == MAGIC_NUM)
    {
        if((*_card)->m_cardName)
        {
            free((*_card)->m_cardName);
        }

        (*_card)->m_magicNumber = 0;
        free(*_card);
        *_card = NULL;
    }
}


Rank GetCardRank(Card* _card)
{
    return (_card) ? _card->m_cardRank : WRONG_RANK;
}



Suite GetCardSuite(Card* _card)
{
    return (_card) ? _card->m_cardSuite : WRONG_SUITE;
}



size_t GetCardValue(Card* _card)
{
    return (_card) ? _card->m_cardValue : 0;
}


const char* GetCardName(Card* _card)
{
    return (_card) ? _card->m_cardName : NULL;
}


/* Cards Utillty Functions: */


void SwapCards(Card** _card1, Card** _card2)
{
    Card* tempCard = NULL;

    if(!_card1 || !*_card1 || !_card2 || !*_card2)
    {
        return;
    }

    tempCard = *_card1;
    *_card1 = *_card2;
    *_card2 = tempCard;
}


int CardsSort(Card** _cardsArray, int _sizeOfCardsArray, callbackCardsCompareFunction _callback)
{
	int i, j, arrayChangedFlag = 0, totalIterations = 0;

	if(!_cardsArray || !_callback)
	{
		return 0;
	}

	for(i = 0; i < _sizeOfCardsArray; i++)
	{
		for(j = 0; j < _sizeOfCardsArray - 1 - totalIterations; j++)
		{
			if((*_callback)(_cardsArray[j], _cardsArray[j + 1]))
			{
				SwapCards(&_cardsArray[j], &_cardsArray[j + 1]);
				arrayChangedFlag = 1;
			}
		}

		if(!arrayChangedFlag)
		{
		/* Array already sorted */
			break;
		}

		arrayChangedFlag = 0;
		totalIterations++;
	}

	return 1;
}


/*--------------------------------- End of Main API Functions -------------------------------------*/


/* Validation Functions: */

static int ValidateCreateCardParams(Rank _rank, Suite _suite)
{
    return (TWO <=_rank && _rank <= ACE && HEART <= _suite && _suite <= CLUB) ? 1 : 0;
}

/* Helper Functions: */

static Card* InitializeNewCard(Card** _card, Rank _rank, Suite _suite)
{
    (*_card)->m_cardRank = _rank;
    (*_card)->m_cardSuite = _suite;
    (*_card)->m_cardValue = ObtainCardNumericValue(_rank);
    (*_card)->m_magicNumber = MAGIC_NUM;

    (*_card)->m_cardName = ObtainCardName(_rank, _suite);
    if(!(*_card)->m_cardName)
    {
        DestroyCard(&(*_card));
        return NULL;
    }

    return (*_card);
}


static size_t ObtainCardNumericValue(Rank _rank)
{
    return (size_t)_rank;
}


static char* ObtainCardName(Rank _rank, Suite _suite)
{
    char* cardName = malloc(STRING_SIZE);
    if(!cardName)
    {
        return NULL;
    }

    switch (_rank)
    {
    case TWO:
        strcpy(cardName, "2");
        break;
    case THREE:
        strcpy(cardName, "3");
        break;
    case FOUR:
        strcpy(cardName, "4");
        break;
    case FIVE:
        strcpy(cardName, "5");
        break;
    case SIX:
        strcpy(cardName, "6");
        break;
    case SEVEN:
        strcpy(cardName, "7");
        break;
    case EIGHT:
        strcpy(cardName, "8");
        break;
    case NINE:
        strcpy(cardName, "9");
        break;
    case TEN:
        strcpy(cardName, "10");
        break;
    case JACK:
        strcpy(cardName, "J");
        break;
    case QUEEN:
        strcpy(cardName, "Q");
        break;
    case KING:
        strcpy(cardName, "K");
        break;
    case ACE:
        strcpy(cardName, "A");
        break;
    default:
        break;
    }

    switch (_suite)
    {
    case HEART:
        strcat(cardName, RED_HEART_SYMBOL_CODE);
        break;
     case SPADE:
        strcat(cardName, BLACK_SPADE_SYMBOL_CODE);
        break;
     case DIAMOND:
        strcat(cardName, RED_DIAMOND_SYMBOL_CODE);
        break;
     case CLUB:
        strcat(cardName, BLACK_CLUB_SYMBOL_CODE);
        break;
    default:
        break;
    }

    return cardName;

    /* Card names for a not supported utf-8 system:
    switch (_rank)
    {
    case TWO:
        strcpy(cardName, "Two ");
        break;
    case THREE:
        strcpy(cardName, "Three ");
        break;
    case FOUR:
        strcpy(cardName, "Four ");
        break;
    case FIVE:
        strcpy(cardName, "Five ");
        break;
    case SIX:
        strcpy(cardName, "Six ");
        break;
    case SEVEN:
        strcpy(cardName, "Seven ");
        break;
    case EIGHT:
        strcpy(cardName, "Eight ");
        break;
    case NINE:
        strcpy(cardName, "Nine ");
        break;
    case TEN:
        strcpy(cardName, "Ten ");
        break;
    case JACK:
        strcpy(cardName, "Jack ");
        break;
    case QUEEN:
        strcpy(cardName, "Queen ");
        break;
    case KING:
        strcpy(cardName, "King ");
        break;
    case ACE:
        strcpy(cardName, "Ace ");
        break;
    }

    switch (_suite)
    {
    case HEART:
        strcat(cardName, "Heart");
        break;
     case SPADE:
        strcat(cardName, "Spade");
        break;
     case DIAMOND:
        strcat(cardName, "Diamond");
        break;
     case CLUB:
        strcat(cardName, "Club");
        break;
    }
*/

}