/**
 * @file Card.h
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief Card module header file
 * @version 0.1
 * @date 2021-03-09
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __CARD__H__
#define __CARD__H__

/* Includes: */

#include <stdlib.h> /* size_t */

/* Defines: */

typedef struct Card Card;

typedef int (*callbackCardsCompareFunction)(Card* _firstCard, Card* _secondCard);

typedef enum
{
    TWO = 2,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    TEN,
    JACK,
    QUEEN,
    KING,
    ACE,
    NO_RANK = 50,
    WRONG_RANK = 99
} Rank;

typedef enum
{
    HEART = 1,
    SPADE,
    DIAMOND,
    CLUB,
    NO_SUITE = 50,
    WRONG_SUITE = 99
} Suite;


/*-------------------------------------- Main API Functions: ---------------------------------------*/


/**
 * @brief Creates and returns a pointer to a new Card
 * 
 * @param _rank: The wanted rank of the card
 * @param _suite: The wanted suite of the card
 * 
 * @return [Card*] A pointer to a new Card, else NULL if error: ALLOCATION_ERROR (failed to allocate memory), WRONG_RANK_ERROR, WRONG_SUITE_ERROR
 */
Card* CreateCard(Rank _rank, Suite _suite);


/**
 * @brief Destroys a given Card pointer and puts NULL in it, prevents double DestroyCard attempts (to avoid double free)
 * 
 * @param _card: A memory address of a pointer to a Card to destroy and release its memory
 */
void DestroyCard(Card** _card);


/**
 * @brief Returns the given Card's rank
 * 
 * @param _card: A pointer to a Card
 * 
 * @return [Rank] The Card's rank, else returns WRONG_RANK if error: NULL_PTR_ERROR (a given pointer is NULL)
 */
Rank GetCardRank(Card* _card);


/**
 * @brief Returns the given Card's suite
 * 
 * @param _card: A pointer to a Card
 * 
 * @return [Suite] The Card's suite, else returns WRONG_SUITE if error: NULL_PTR_ERROR (a given pointer is NULL)
 */
Suite GetCardSuite(Card* _card);


/**
 * @brief Returns the given Card's numeric value
 * 
 * @param _card: A pointer to a Card
 *
 * @return [size_t] The Card's numeric value, else returns 0 if error: NULL_PTR_ERROR (a given pointer is NULL)
 */
size_t GetCardValue(Card* _card);


/**
* @brief Gets the string name of a given Card
 * 
 * @param _player: A pointer to a Card
 * 
 * @return [const char*] The name of the Card as a string, else NULL if error: NULL_PTR_ERROR (a given pointer is NULL)
 */
const char* GetCardName(Card* _card);


/* Cards Utillty Functions: */


/**
 * @brief Swaps the value of two given Cards (swapping them by swapping their memory addresses)
 * 
 * @param _card1: A pointer to a Card to swap
 * @param _card2: A pointer to a Card to swap
 */
void SwapCards(Card** _card1, Card** _card2);


/**
 * @brief Sorts a given Card pointers array by invoking a given callback compare function for Cards
 * 
 * @param _cardsArray: Array of pointers to Cards to sort
 * @param _sizeOfCardsArray: THe size of the given array
 * @param _callback: A callback compare function to invoke in the sort function and compare between two Cards, should return 1 (true) or 0 (false)
 *
 * @return [int] 1 if succeed, else 0 if error: NULL_PTR_ERROR (a given pointer is NULL)
 */
int CardsSort(Card** _cardsArray, int _sizeOfCardsArray, callbackCardsCompareFunction _callback);


/*--------------------------------- End of Main API Functions -------------------------------------*/

#endif  /* #ifndef __CARD__H__ */