/**
 * @file Deck.h
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief Deck module header file
 * @version 0.1
 * @date 2021-03-09
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __DECK_H__
#define __DECK_H__

#include "../../Modules/Headers/Card.h"

/* Defines: */

typedef struct Deck Deck;


/*-------------------------------------- Main API Functions: ---------------------------------------*/


/**
 * @brief Creates and returns a pointer to a new Deck
 * 
 * @param _numberOfPacks: Number of packs of cards to combine in the single Deck, must be at least 1
 * 
 * @return [Deck*] A pointer to a new Deck, else NULL if error: ALLOCATION_ERROR (failed to allocate memory), WRONG_PACKS_COUNT
 */
Deck* CreateDeck(size_t _numberOfPacks);


/**
 * @brief Destroys a given Deck pointer and puts NULL in it, prevents double DestroyDeck attempts (to avoid double free)
 * 
 * @param _deck: A memory address of a pointer to a Deck to destroy and release its memory
 */
void DestroyDeck(Deck** _deck);


/**
 * @brief Draws a Card from the top of the given Deck
 * 
 * @param _deck: A pointer to a Deck
 * 
 * @return [Card*] A pointer to the drawn Card, else NULL if error: NULL_PTR_ERROR (a given pointer is NULL), EMPTY_DECK_ERROR
 */
Card* DrawCardFromDeck(Deck* _deck);


/**
* @brief Returns a given Card to the bottom of the given Deck
 * 
 * @param _deck: A pointer to a Deck
 * @param _cardToReturn: A pointer to a Card to return to the Deck
 * 
 * @return [int] 1 if succeed, else 0 if error: NULL_PTR_ERROR (a given pointer is NULL), FULL_DECK_ERROR
 */
int ReturnCardToDeck(Deck* _deck, Card* _cardToReturn);


/**
 * @brief Shuffles a given Deck
 * 
 * @param _deck: A pointer to a Deck
 * 
 * @return [int] 1 if succeed, else 0 if error: NULL_PTR_ERROR (a given pointer is NULL)
 */
int ShuffleDeck(Deck* _deck);


/**
 * @brief Checks if a given Deck is empty or not
 * 
 * @param _deck: A pointer to a Deck
 *
 * @return [int] 1 if Deck is empty, 0 if Deck is not empty, else -1 if error: NULL_PTR_ERROR (a given pointer is NULL)
 */
int IsEmptyDeck(Deck* _deck);


/**
 * @brief Gets the current cards count in the given Deck
 * 
 * @param _deck: A pointer to a Deck
 *
 * @return [size_t] The current cards count in the Deck, else 0 if error: NULL_PTR_ERROR (a given pointer is NULL)
 */
size_t GetCurrentDeckCardsCount(Deck* _deck);


/*--------------------------------- End of Main API Functions -------------------------------------*/

#endif /* #ifndef __DECK_H__ */