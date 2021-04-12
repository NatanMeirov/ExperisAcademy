/**
 * @file Hand.h
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief Hand module header file
 * @version 0.1
 * @date 2021-03-09
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __HAND_H__
#define __HAND_H__

#include "../../Modules/Headers/Card.h"

/* Defines: */

typedef struct Hand Hand;


/*-------------------------------------- Main API Functions: ---------------------------------------*/


/**
 * @brief Creates and returns a pointer to a new Hand, initialized with a given maximum amount of cards for a game
 * 
 * @param _maxCardsInHand: A maximum amount of cards in Hand for a game, must be at least 1
 * 
 * @return [Hand*] A pointer to a new Hand, else NULL if error: ALLOCATION_ERROR (failed to allocate memory)
 */
Hand* CreateHand(size_t _maxCardsInHand);


/**
 * @brief Destroys a given Hand pointer and puts NULL in it, prevents double DestroyHand attempts (to avoid double free)
 * 
 * @param _hand: A memory address of a pointer to a Hand to destroy and release its memory
 */
void DestroyHand(Hand** _hand);


/**
 * @brief Takes a given Card to hold it in the Hand
 * 
 * @param _hand: A pointer to a Hand
 * @param _cardToTake: A pointer to a Card to hold in the Hand
 * 
 * @return [int] 1 if succeed, else 0 if error: NULL_PTR_ERROR (a given pointer is NULL), FULL_HAND_ERROR
 */
int TakeCardToHoldInHand(Hand* _hand, Card* _cardToTake);


/**
 * @brief Returns (puts away) a wanted Card, found by its rank and/or its suite from the Hand, the Hand will be sorted by ranks after the pick
 * 
 * @param _hand: A pointer to a Hand
 * @param _rank: The rank of the Card to get from the Hand (if should be ignored - pass NO_RANK as _rank [cannot be both: NO_RANK and NO_SUITE])
 * @param _suite: The suite of the Card to get from the Hand (if should be ignored - pass NO_SUITE as _suite [cannot be both: NO_RANK and NO_SUITE])
 * 
 * @return [Card*] A pointer to a Card to put away, else NULL if error: NULL_PTR_ERROR (a given pointer is NULL), WRONG_CARD_SPECIFIERS, EMPTY_HAND_ERROR
 */
Card* GetCardFromHand(Hand* _hand, Rank _rank, Suite _suite);


/**
 * @brief Gets the highest Card by its numeric value from the Hand, without considering its suite, the Hand will be sorted by ranks after the pick
 * 
 * @param _hand: A pointer to a Hand
 * 
 * @return [Card*] A pointer to a Card to put away, else NULL if error: NULL_PTR_ERROR (a given pointer is NULL), EMPTY_HAND_ERROR
 */
Card* GetHighestCardByRank(Hand* _hand);


/**
 * @brief Gets the highest Card instance of a given suite from the Hand, the Hand will be sorted by ranks after the pick
 * 
 * @param _hand: A pointer to a Hand
 * @param _suite: The suite of the Card to get its highest instance from the Hand
 * 
 * @return [Card*] A pointer to a Card to put away, else NULL if error: NULL_PTR_ERROR (a given pointer is NULL), EMPTY_HAND_ERROR
 */
Card* GetHighestCardBySuite(Hand* _hand, Suite _suite);


/**
 * @brief Gets the lowest Card by its numeric value from the Hand, without considering its suite, the Hand will be sorted by ranks after the pick
 * 
 * @param _hand: A pointer to a Hand
 * 
 * @return [Card*] A pointer to a Card to put away, else NULL if error: NULL_PTR_ERROR (a given pointer is NULL), EMPTY_HAND_ERROR
 */
Card* GetLowestCardByRank(Hand* _hand);


/**
 * @brief Gets the lowest Card instance of a given suite from the Hand, the Hand will be sorted by ranks after the pick
 * 
 * @param _hand: A pointer to a Hand
 * @param _suite: The suite of the Card to get its lowest instance from the Hand
 * 
 * @return [Card*] A pointer to a Card to put away, else NULL if error: NULL_PTR_ERROR (a given pointer is NULL), EMPTY_HAND_ERROR
 */
Card* GetLowestCardBySuite(Hand* _hand, Suite _suite);


/**
 * @brief Sorts a given Hand by ranks of its cards, without keeping the order of their suites
 * 
 * @param _hand: A pointer to a Hand
 * 
 * @return [int] 1 if succeed, else 0 if error: NULL_PTR_ERROR (a given pointer is NULL)
 */
int SortHandByRanks(Hand* _hand);


/**
 * @brief Sorts a given Hand by ranks of its cards, keeping the the order of their suites
 * 
 * @param _hand: A pointer to a Hand
 * 
 * @return [int] 1 if succeed, else 0 if error: NULL_PTR_ERROR (a given pointer is NULL)
 */
int SortHandByRanksAndSuites(Hand* _hand);


/**
 * @brief Checks if a given Hand has a specific Card or not, by its rank and suite
 * 
 * @param _hand: A pointer to a Hand
 * @param _rank: The rank of the Card to check (if should be ignored - pass NO_RANK as _rank [cannot be both: NO_RANK and NO_SUITE])
 * @param _suite: The suite of the Card to check (if should be ignored - pass NO_SUITE as _suite [cannot be both: NO_RANK and NO_SUITE])
 * 
 * @return [int] 1 if then Card is in Hand, 0 if the Card is not in Hand, else -1 if error: NULL_PTR_ERROR (a given pointer is NULL), WRONG_RANK_ERROR, WRONG_SUITE_ERROR, EMPTY_HAND_ERROR
 */
int IsCardInHand(Hand* _hand, Rank _rank, Suite _suite);


/**
 * @brief Checks if a given Hand is empty or not
 * 
 * @param _hand: A pointer to a Hand
 *
 * @return [int] 1 if Hand is empty, 0 if Hand is not empty, else -1 if error: NULL_PTR_ERROR (a given pointer is NULL)
 */
int IsEmptyHand(Hand* _hand);


/**
 * @brief Shuffles the Cards of a given Hand to mess their order
 * 
 * @param _hand: A pointer to a Hand
 * 
 * @return [int] 1 if succeed, else 0 if error: NULL_PTR_ERROR (a given pointer is NULL)
 */
int ShuffleCardsInHand(Hand* _hand);


/**
 * @brief Gets a string representation of all the Cards' names of the given Hand
 * 
 * @param _hand: A pointer to a Hand
 * 
 * @return [const char*] All the Cards' names from the Hand as a long string, else NULL if error: NULL_PTR_ERROR (a given pointer is NULL), ALLOCATION_ERROR (failed to allocate memory)
 */
const char* GetAllCardsNamesFromHand(Hand* _hand);


/**
 * @brief Gets the number of the Cards In the given Hand
 * 
 * @param _hand: A pointer to a Hand
 * 
 * @return [size_t] The current cards count in the Hand, else 0 if error: NULL_PTR_ERROR (a given pointer is NULL)
 */
size_t GetNumberOfCardsInHand(Hand* _hand);


/*--------------------------------- End of Main API Functions -------------------------------------*/

#endif /* #ifndef __HAND_H__ */