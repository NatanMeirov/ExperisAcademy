/**
 * @file CardsGamePlayer.h
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief CardsGamePlayer module header file
 * @version 0.1
 * @date 2021-03-09
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __CARDSGAMEPLAYER__H__
#define __CARDSGAMEPLAYER__H__

/* Includes: */

#include <stddef.h> /* size_t */
#include "../../Modules/Headers/Hand.h"
#include "../../Modules/Headers/Card.h"

/* Defines: */

typedef struct CardsGamePlayer CardsGamePlayer;

typedef Card* (*SingleCardStrategyFunction)(Hand* _playerHand, Rank _rank, Suite _suite);
typedef Card** (*MultipleCardsStrategyFunction)(Hand* _playerHand, Rank _rank, Suite _suite);


/*-------------------------------------- Main API Functions: ---------------------------------------*/


/**
 * @brief Creates and returns a pointer to a new CardsGamePlayer
 * 
 * @param _playerName: The name of the new CardsGamePlayer
 * @param _isRealPlayer: A boolean flag to determine if the new player is a real player or a computer player
 * 
 * @return [CardsGamePlayer*] A pointer to a new CardsGamePlayer, else NULL if error: ALLOCATION_ERROR (failed to allocate memory), NULL_PTR_ERROR (a given pointer is NULL)
 */
CardsGamePlayer* CreateCardsGamePlayer(const char* _playerName, int _isRealPlayer);


/**
 * @brief Destroys a given CardsGamePlayer pointer and puts NULL in it, prevents double DestroyHand attempts (to avoid double free)
 * 
 * @param _player: A memory address of a pointer to a CardsGamePlayer to destroy and release its memory
 */
void DestroyCardsGamePlayer(CardsGamePlayer** _player);


/**
 * @brief Resets the given CardsGamePlayer's Hand, and gives a new maximum amount of cards to hold in its Hand
 * 
 * @param _player: A pointer to a CardsGamePlayer
 * @param _maxCardsInHand: The maximum amount of Cards of the CardsGamePlayer, must be at least 1
 * 
 * @return [int] 1 if succeed, else 0 if error: NULL_PTR_ERROR (a given pointer is NULL)
 */
int ResetCardsGamePlayerHand(CardsGamePlayer* _player, size_t _maxCardsInHand);


/**
 * @brief Gets a single Card from a CardsGamePlayer, triggers a Strategy Function to get the Card
 * 
 * @param _player: A pointer to a CardsGamePlayer
 * @param _strategy: A pointer to a Strategy Function to trigger to get a Card, should be a specific strategy for a real player (like contacting through the UI), and another strategy for a computer player (Artificial Intelligence process)
 * @param _rank: The rank of the Card to get from the Hand (if should be ignored - pass NO_RANK as _rank)
 * @param _suite: The suite of the Card to get from the Hand (if should be ignored - pass NO_SUITE as _suite)
 * 
 * @return [Card*] A pointer to the a Card of the CardsGamePlayer, else NULL if error: NULL_PTR_ERROR (a given pointer is NULL)
 */
Card* GetSingleCardFromCardsGamePlayer(CardsGamePlayer* _player, SingleCardStrategyFunction _strategy, Rank _rank, Suite _suite);


/**
 * @brief Gets multiple Cards from a CardsGamePlayer, triggers a Strategy Function to get the Cards
 * 
 * @param _player: A pointer to a CardsGamePlayer
 * @param _strategy: A pointer to a Strategy Function to trigger to get the Cards, should be a specific strategy for a real player (like contacting through the UI), and another strategy for a computer player (Artificial Intelligence process)
 * @param _rank: The rank of the Card to get from the Hand (if should be ignored - pass NO_RANK as _rank)
 * @param _suite: The suite of the Card to get from the Hand (if should be ignored - pass NO_SUITE as _suite)
 * 
 * @return [Card**] An array of pointers to the Cards of the CardsGamePlayer, else NULL if error: NULL_PTR_ERROR (a given pointer is NULL)
 */
Card** GetMultipleCardsFromCardsGamePlayer(CardsGamePlayer* _player, MultipleCardsStrategyFunction _strategy, Rank _rank, Suite _suite);


/**
 * @brief Sets a single Card to a CardsGamePlayer
 * 
 * @param _player: A pointer to a CardsGamePlayer
 * @param _cardToGive: A Card to give to the CardsGamePlayer
 * 
 * @return [int] 1 if succeed, else 0 if error: NULL_PTR_ERROR (a given pointer is NULL)
 */
int GiveSingleCardToCardsGamePlayer(CardsGamePlayer* _player, Card* _cardToGive);


/**
 * @brief Gets the string name of a given CardsGamePlayer
 * 
 * @param _player: A pointer to a CardsGamePlayer
 * 
 * @return [const char*] The name of the CardsGamePlayer as a string, else NULL if error: NULL_PTR_ERROR (a given pointer is NULL)
 */
const char* GetNameOfCardsGamePlayer(CardsGamePlayer* _player);


/**
 * @brief Sets the score to a given CardsGamePlayer
 * 
 * @param _player: A pointer to a CardsGamePlayer
 * @param _score: The score to set
 * 
 * @return [int] 1 if succeed, else 0 if error: NULL_PTR_ERROR (a given pointer is NULL)
 */
int SetScoreOfCardsGamePlayer(CardsGamePlayer* _player, int score);


/**
 * @brief Gets the score of a given CardsGamePlayer
 * 
 * @param _player: A pointer to a CardsGamePlayer
 * 
 * @return [int] The CardsGamePlayer's score, else -1 if error: NULL_PTR_ERROR (a given pointer is NULL)
 */
int GetScoreOfCardsGamePlayer(CardsGamePlayer* _player);


/**
 * @brief Checks if a given CardsGamePlayer is a real player or not
 * 
 * @param _player: A pointer to a CardsGamePlayer
 * 
 * @return [int] 1 if the CardsGamePlayer is a real player, 0 if the CardsGamePlayer is a Computer Player, else -1 if error: NULL_PTR_ERROR (a given pointer is NULL)
 */
int IsRealPlayer(CardsGamePlayer* _player);


/**
 * @brief Checks if a given CardsGamePlayer has a specific Card or not, by its rank and suite, can also be specified only by its rank or its suite
 * 
 * @param _player: A pointer to a CardsGamePlayer
 * @param _rank: The rank of the Card to check (if should be ignored - pass NO_RANK as _rank [cannot be both: NO_RANK and NO_SUITE])
 * @param _suite: The suite of the Card to check (if should be ignored - pass NO_SUITE as _suite [cannot be both: NO_RANK and NO_SUITE])
 * 
 * @return [int] 1 if the CardsGamePlayer has the specific card, 0 if not, else -1 if error: NULL_PTR_ERROR (a given pointer is NULL), WRONG_RANK_ERROR, WRONG_SUITE_ERROR, NO_CARDS_FOR_PLAYER_ERROR
 */
int IsPlayerHoldSpecificCard(CardsGamePlayer* _player, Rank _rank, Suite _suite);


/**
 * @brief Gets the number of the Cards that the CardsGamePlayer is holding
 * 
 * @param _player: A pointer to a CardsGamePlayer
 * 
 * @return [size_t] The current cards count of the CardsGamePlayer, else 0 if error: NULL_PTR_ERROR (a given pointer is NULL)
 */
size_t GetNumberOfCardsPlayerIsHolding(CardsGamePlayer* _player);


/*--------------------------------- End of Main API Functions -------------------------------------*/


#endif  /* #ifndef __CARDSGAMEPLAYER__H__ */