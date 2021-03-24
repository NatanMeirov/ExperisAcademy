/**
 * @file Deck.c
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief Deck module source file
 * @version 0.1
 * @date 2021-03-10
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Includes: */

#include <stdlib.h> /* malloc, free, size_t, rand, rands */
#include <time.h> /* time */
#include "../../Modules/Headers/Deck.h"
#include "../../Modules/Headers/Card.h"


/* Defines: */

#define MAGIC_NUM 47781551
#define CARDS_PACK_SIZE 52
#define RANKS_COUNT 13
#define SUITES_COUNT 4
#define SWAPPING_TIMES 5


struct Deck
{
    Card** m_deckCards;
    int m_deckTopCard;
    int m_magicNumber;
};

/* Validation Functions Declarations: */

static int ValidateCreateDeckParams(size_t _numberOfPacks);

/* Helper Functions Declarations: */

static Deck* InitializeNewDeck(Deck** _deck, size_t _numberOfPacks);


/*-------------------------------------- Main API Functions: ---------------------------------------*/


Deck* CreateDeck(size_t _numberOfPacks)
{
    Deck* newDeck = NULL;

    if(!ValidateCreateDeckParams(_numberOfPacks))
    {
        return NULL;
    }

    newDeck = malloc(sizeof(Deck));
    if(!newDeck)
    {
        return NULL;
    }

    return InitializeNewDeck(&newDeck, _numberOfPacks);
}


void DestroyDeck(Deck** _deck)
{
    int i; /* using int instead of size_t because m_deckTopCard could have negative value - and if it will be converted to size_t - it will loop until MAX_LONG (or something like that) */

    if(_deck && *_deck && (*_deck)->m_magicNumber == MAGIC_NUM)
    {
        if((*_deck)->m_deckCards) /* Optionally to be triggered at the initialization stage - a chance that this pointer is not successfully allocated */
        {
            for(i = 0; i <= (*_deck)->m_deckTopCard; i++)
            {
                DestroyCard(&((*_deck)->m_deckCards[i]));
            }

            free((*_deck)->m_deckCards);
        }

        (*_deck)->m_magicNumber = 0;
        free(*_deck);
        *_deck = NULL;
    }
}


Card* DrawCardFromDeck(Deck* _deck)
{
    Card* card = NULL;

    if(_deck->m_deckTopCard == -1)
    {
        return NULL;
    }

    card = _deck->m_deckCards[_deck->m_deckTopCard];
    _deck->m_deckCards[_deck->m_deckTopCard] = NULL;
    _deck->m_deckTopCard--;
    return card;
}


int ReturnCardToDeck(Deck* _deck, Card* _cardToReturn)
{
    /*TODO: implement a shift to right function to set the returned Card in the index of 0 (bottom of the Deck),
    Check if not full deck */

    return 0;
}


int ShuffleDeck(Deck* _deck)
{
    size_t i, cardsSwappingTime;

    if(!_deck)
    {
        return 0;
    }

    cardsSwappingTime = (size_t)(_deck->m_deckTopCard + 1) * SWAPPING_TIMES;

    srand(time(NULL));

    for(i = 0; i < cardsSwappingTime; i++)
    {
        SwapCards(&(_deck->m_deckCards[rand() % (_deck->m_deckTopCard + 1)]), &(_deck->m_deckCards[rand() % (_deck->m_deckTopCard + 1)]));
    }

    return 1;
}


int IsEmptyDeck(Deck* _deck)
{
    if(!_deck)
    {
        return -1;
    }

    return (_deck->m_deckTopCard == -1) ? 1 : 0;
}


size_t GetCurrentDeckCardsCount(Deck* _deck)
{
    if(!_deck)
    {
        return 0;
    }

    return (_deck->m_deckTopCard + 1);
}


/*--------------------------------- End of Main API Functions -------------------------------------*/


/* Validation Functions: */

static int ValidateCreateDeckParams(size_t _numberOfPacks)
{
    return (_numberOfPacks > 0) ? 1 : 0;
}

/* Helper Functions: */

static Deck* InitializeNewDeck(Deck** _deck, size_t _numberOfPacks)
{
    size_t i, j, k;
    Rank ranks[] = {TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, ACE};
    Suite suites[] = {HEART, SPADE, DIAMOND, CLUB};
    Card* newCard = NULL;

    (*_deck)->m_deckCards = malloc(sizeof(Card*) * _numberOfPacks * CARDS_PACK_SIZE);
    if(!(*_deck)->m_deckCards)
    {
        DestroyDeck(&(*_deck));
        return NULL;
    }

    (*_deck)->m_deckTopCard = -1; /* THe Deck is still empty - this index will always point to the top Card in the Deck*/

    for(i = 0; i < _numberOfPacks; i++)
    {
        for(j = 0; j < SUITES_COUNT; j++)
        {
            for(k = 0; k < RANKS_COUNT; k++)
            {
                newCard = CreateCard(ranks[k], suites[j]);
                if(!newCard)
                {
                    DestroyDeck(&(*_deck));
                    return NULL;
                }

                (*_deck)->m_deckTopCard++;
                (*_deck)->m_deckCards[(*_deck)->m_deckTopCard] = newCard;
            }
        }
    }

    (*_deck)->m_magicNumber = MAGIC_NUM;

    return (*_deck);
}