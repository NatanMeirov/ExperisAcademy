/**
 * @file CardsGameRound.h
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief CardsGameRound module header file
 * @version 0.1
 * @date 2021-03-10
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __CARDSGAMEROUND__H__
#define __CARDSGAMEROUND__H__

/* Includes: */

#include <stdlib.h> /* size_t */
#include "../../Modules/Headers/CardsGamePlayer.h"

/* Defines: */

typedef struct CardsGameRound CardsGameRound;


/*-------------------------------------- Main API Functions: ---------------------------------------*/


/**
 * @brief Creates and returns a pointer to a new CardsGameRound
 * 
 * @param _players: An array of pointers to CardsGamePlayers that should participate in the CardsGameRound
 * @param _playersCount: The number of the CardsGamePlayers in the CardsGameRound, must be at least 2
 * 
 * @return [CardsGameRound*] A pointer to a new CardsGameRound, else NULL if error: NULL_PTR_ERROR (a given pointer is NULL), ALLOCATION_ERROR (failed to allocate memory), WRONG_PLAYERS_COUNT
 */
CardsGameRound* CreateCardsGameRound(CardsGamePlayer** _players, size_t _playersCount);


/**
 * @brief Destroys a given CardsGameRound pointer and puts NULL in it, prevents double DestroyCardsGameRound attempts (to avoid double free)
 * 
 * @param _game: A memory address of a pointer to a CardsGameRound to destroy and release its memory
 */
void DestroyCardsGameRound(CardsGameRound** _round);


/**
 * @brief Runs a given CardsGameRound's engine
 * 
 * @param _round: A pointer to a CardsGameRound to run
 * @param _scoreBoard: An array of CardsGamePlayers scores to update in the given CardsGameRound by reference
 * @param _roundNumber: The current round number of the CardsGameRound to launch
 * 
 * @return [int] 1 if succeed, else 0 if error: NULL_PTR_ERROR (a given pointer is NULL)
 */
int RunCardsGameRoundEngine(CardsGameRound* _round, int* _scoreBoard, size_t _roundNumber);


/*--------------------------------- End of Main API Functions -------------------------------------*/


#endif  /* #ifndef __CARDSGAMEROUND__H__ */