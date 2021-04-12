/**
 * @file Hand.c
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief Hand module source file
 * @version 0.1
 * @date 2021-03-10
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Includes: */

#include <stdlib.h> /* malloc, free, size_t */
#include <string.h> /* strcpy, strcat, strlen */
#include <time.h> /* time */
#include "../../Modules/Headers/Hand.h"
#include "../../Modules/Headers/Card.h"

/* Defines: */

#define MAGIC_NUM 18725344
#define MAX_BUFFER_LENGTH 300

struct Hand
{
    Card** m_handCards;
    size_t m_maxCardsInHand;
    size_t m_numOfCardsInHand;
    char* m_handCardsNames; /* Lazy initialization - updates on every get function call */
    int m_magicNumber;
};


/* Validation Functions Declarations: */

static int ValidateCreateHandParams(size_t _maxCardsInHand);

/* Helper Functions Declarations: */

static Hand* InitializeNewHand(Hand** _hand, size_t _maxCardsInHand);
static void ShiftCardsToLeft(Card** _handCards, size_t _indexToThread, size_t _lastCardIndex);
static int FindCardBySuite(Hand* _hand, Suite _suite);
static int FindCardByRank(Hand* _hand, Rank _rank);
static int FindCardByRankAndSuite(Hand* _hand, Rank _rank, Suite _suite);
int ConditionToSortCardsByNumericValue(Card* _card1, Card* _card2);
int ConditionToSortCardsByRanksAndSuites(Card* _card1, Card* _card2);


/*-------------------------------------- Main API Functions: ---------------------------------------*/


Hand* CreateHand(size_t _maxCardsInHand)
{
    Hand* newHand = NULL;

    if(!ValidateCreateHandParams(_maxCardsInHand))
    {
        return NULL;
    }

    newHand = malloc(sizeof(Hand));
    if(!newHand)
    {
        return NULL;
    }

    return InitializeNewHand(&newHand, _maxCardsInHand);
}


void DestroyHand(Hand** _hand)
{
    size_t i;

    if(_hand && *_hand && (*_hand)->m_magicNumber == MAGIC_NUM)
    {
        if((*_hand)->m_handCards)  /* Optionally to be triggered at the initialization stage - a chance that this pointer is not successfully allocated */
        {
            for(i = 0; i < (*_hand)->m_numOfCardsInHand; i++)
            {
                DestroyCard(&((*_hand)->m_handCards[i]));
            }

            free((*_hand)->m_handCards);
        }

        if((*_hand)->m_handCardsNames)
        {
            free((*_hand)->m_handCardsNames);
        }

        (*_hand)->m_magicNumber = 0;
        free(*_hand);
        *_hand = NULL;
    }
}


int TakeCardToHoldInHand(Hand* _hand, Card* _cardToTake)
{
    if(!_hand || !_cardToTake || (_hand->m_numOfCardsInHand == _hand->m_maxCardsInHand))
    {
        return 0;
    }

    _hand->m_handCards[_hand->m_numOfCardsInHand] = _cardToTake;
    _hand->m_numOfCardsInHand++;

    return 1;
}


Card* GetCardFromHand(Hand* _hand, Rank _rank, Suite _suite)
{
    Card* card = NULL;
    int cardIndex;

    if(!_hand || _hand->m_numOfCardsInHand == 0 || (_rank == NO_RANK && _suite == NO_SUITE))
    {
        return NULL;
    }

    ShuffleCardsInHand(_hand); /* To pick a card by its suite or rank without considering their order (to avoid picking always the highest or smallest card) */

    if(_rank == NO_RANK)
    {
        cardIndex = FindCardBySuite(_hand, _suite);
    }
    else if(_suite == NO_SUITE)
    {
        cardIndex = FindCardByRank(_hand, _rank);
    }
    else
    {
        cardIndex = FindCardByRankAndSuite(_hand, _rank, _suite);
    }

    if(cardIndex == -1)
    {
        return NULL; /* Card not found */
    }

    card = _hand->m_handCards[cardIndex];
    ShiftCardsToLeft(_hand->m_handCards, (size_t)cardIndex, _hand->m_numOfCardsInHand - 1);
    _hand->m_numOfCardsInHand--;

    SortHandByRanks(_hand); /* Sort the hand again after the shuffle */

    return card;
}


Card* GetHighestCardByRank(Hand* _hand)
{
    Card* card = NULL;

    if(!_hand || _hand->m_numOfCardsInHand == 0)
    {
        return NULL;
    }

    SortHandByRanks(_hand);
    card = _hand->m_handCards[_hand->m_numOfCardsInHand - 1];
    _hand->m_handCards[_hand->m_numOfCardsInHand - 1] = NULL;
    _hand->m_numOfCardsInHand--;

    return card;
}


Card* GetHighestCardBySuite(Hand* _hand, Suite _suite)
{
    size_t i;
    Card* card = NULL;

    if(!_hand)
    {
        return NULL;
    }

    SortHandByRanks(_hand);

    for(i = _hand->m_numOfCardsInHand; i > 0; i--)
    {
        if(GetCardSuite(_hand->m_handCards[i - 1]) == _suite)
        {
            card = _hand->m_handCards[i - 1];
            break;
        }
    }

    if(!card)
    {
        return NULL; /* Suite not found */
    }

    ShiftCardsToLeft(_hand->m_handCards, i - 1, _hand->m_numOfCardsInHand - 1);
    _hand->m_numOfCardsInHand--;

    return card;
}


Card* GetLowestCardByRank(Hand* _hand)
{
    Card* card = NULL;

    if(!_hand || _hand->m_numOfCardsInHand == 0)
    {
        return NULL;
    }

    SortHandByRanks(_hand);
    card = _hand->m_handCards[0];
    _hand->m_handCards[0] = _hand->m_handCards[_hand->m_numOfCardsInHand - 1];
    _hand->m_handCards[_hand->m_numOfCardsInHand - 1] = NULL;
    _hand->m_numOfCardsInHand--;
    SortHandByRanks(_hand);

    return card;
}


Card* GetLowestCardBySuite(Hand* _hand, Suite _suite)
{
    size_t i;
    Card* card = NULL;

    if(!_hand)
    {
        return NULL;
    }

    SortHandByRanks(_hand);

    for(i = 0; i <_hand->m_numOfCardsInHand; i++)
    {
        if(GetCardSuite(_hand->m_handCards[i]) == _suite)
        {
            card = _hand->m_handCards[i];
            break;
        }
    }

    if(!card)
    {
        return NULL; /* Suite not found */
    }

    ShiftCardsToLeft(_hand->m_handCards, i, _hand->m_numOfCardsInHand - 1);
    _hand->m_numOfCardsInHand--;

    return card;
}


int SortHandByRanks(Hand* _hand)
{
    if(!_hand)
    {
        return 0;
    }

    return CardsSort(_hand->m_handCards, _hand->m_numOfCardsInHand, &ConditionToSortCardsByNumericValue);
}


int SortHandByRanksAndSuites(Hand* _hand)
{
    if(!_hand)
    {
        return 0;
    }

    return CardsSort(_hand->m_handCards, _hand->m_numOfCardsInHand, &ConditionToSortCardsByRanksAndSuites);
}


int IsCardInHand(Hand* _hand, Rank _rank, Suite _suite)
{
    if(!_hand || (_rank == NO_RANK && _suite == NO_SUITE))
    {
        return -1;
    }

    if(_rank == NO_RANK)
    {
        return (FindCardBySuite(_hand, _suite) != -1) ? 1 : 0;
    }

    if(_suite == NO_SUITE)
    {
        return (FindCardByRank(_hand, _rank) != -1) ? 1 : 0;
    }

    return (FindCardByRankAndSuite(_hand, _rank, _suite) != -1) ? 1 : 0;
}


int IsEmptyHand(Hand* _hand)
{
    if(!_hand)
    {
        return -1;
    }

    return (_hand->m_numOfCardsInHand == 0) ? 1 : 0;
}


int ShuffleCardsInHand(Hand* _hand)
{
    size_t i, cardsSwappingTime;

    if(!_hand)
    {
        return 0;
    }

    cardsSwappingTime = _hand->m_numOfCardsInHand * 2;

    srand(time(NULL));

    for(i = 0; i < cardsSwappingTime; i++)
    {
        SwapCards(&(_hand->m_handCards[rand() % (_hand->m_numOfCardsInHand)]), &(_hand->m_handCards[rand() % (_hand->m_numOfCardsInHand)]));
    }

    return 1;
}


const char* GetAllCardsNamesFromHand(Hand* _hand)
{
    size_t i;

    if(!_hand)
    {
        return NULL;
    }

    strcpy(_hand->m_handCardsNames, "Cards in Hand:\n");

    for(i = 0; i < _hand->m_numOfCardsInHand; i++)
    {
        strncat(_hand->m_handCardsNames, GetCardName(_hand->m_handCards[i]), (MAX_BUFFER_LENGTH - strlen(_hand->m_handCardsNames))); /* For safety - checks every iteration if there is more space in the buffer */
        strncat(_hand->m_handCardsNames, "   ", (MAX_BUFFER_LENGTH - strlen(_hand->m_handCardsNames)));
    }

    return _hand->m_handCardsNames;
}


size_t GetNumberOfCardsInHand(Hand* _hand)
{
    if(!_hand)
    {
        return 0;
    }

    return _hand->m_numOfCardsInHand;
}


/*--------------------------------- End of Main API Functions -------------------------------------*/


/* Validation Functions: */

static int ValidateCreateHandParams(size_t _maxCardsInHand)
{
    return (_maxCardsInHand > 0) ? 1 : 0;
}

/* Helper Functions: */

static Hand* InitializeNewHand(Hand** _hand, size_t _maxCardsInHand)
{
    (*_hand)->m_handCards = malloc(sizeof(Card*) * _maxCardsInHand);
    if(!(*_hand)->m_handCards)
    {
        DestroyHand(&(*_hand));
        return NULL;
    }

    (*_hand)->m_handCardsNames = malloc(MAX_BUFFER_LENGTH);
    if(!(*_hand)->m_handCardsNames)
    {
        DestroyHand(&(*_hand));
        return NULL;
    }

    (*_hand)->m_maxCardsInHand = _maxCardsInHand;
    (*_hand)->m_numOfCardsInHand = 0; /* Current number of Cards in Hand */
    (*_hand)->m_magicNumber = MAGIC_NUM;

    return (*_hand);
}


static void ShiftCardsToLeft(Card** _handCards, size_t _indexToThread, size_t _lastCardIndex)
{
    size_t i;

    for(i = _indexToThread; i  < _lastCardIndex; i++)
    {
        _handCards[i] = _handCards[i + 1];
    }

    _handCards[_lastCardIndex] = NULL;
}


static int FindCardBySuite(Hand* _hand, Suite _suite)
{
    size_t i;

    for(i = 0; i < _hand->m_numOfCardsInHand; i++)
    {
        if(GetCardSuite(_hand->m_handCards[i]) == _suite)
        {
            return i;
        }
    }

    return -1; /* Card not found */
}


static int FindCardByRank(Hand* _hand, Rank _rank)
{
    size_t i;

    for(i = 0; i < _hand->m_numOfCardsInHand; i++)
    {
        if(GetCardRank(_hand->m_handCards[i]) == _rank)
        {
            return i;
        }
    }

    return -1; /* Card not found */
}


static int FindCardByRankAndSuite(Hand* _hand, Rank _rank, Suite _suite)
{
    size_t i;

    for(i = 0; i < _hand->m_numOfCardsInHand; i++)
    {
        if(GetCardRank(_hand->m_handCards[i]) == _rank && GetCardSuite(_hand->m_handCards[i]) == _suite)
        {
            return i;
        }
    }

    return -1; /* Card not found */
}


int ConditionToSortCardsByNumericValue(Card* _card1, Card* _card2)
{
    return (GetCardValue(_card1) > GetCardValue(_card2)) ? 1 : 0;
}


int ConditionToSortCardsByRanksAndSuites(Card* _card1, Card* _card2)
{
    /* Using a unique calculation to compare the given cards in an inner-suite sorted order, but in general partition between the four suites */
    size_t comparableCard1NewValue = GetCardValue(_card1) + ((size_t)GetCardSuite(_card1) * 20);
    size_t comparableCard2NewValue = GetCardValue(_card2) + ((size_t)GetCardSuite(_card2) * 20);

    return (comparableCard1NewValue >  comparableCard2NewValue) ? 1 : 0;
}