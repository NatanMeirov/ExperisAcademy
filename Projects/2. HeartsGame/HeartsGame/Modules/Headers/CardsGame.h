/**
 * @file CardsGame.h
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief CardsGame module header file
 * @version 0.1
 * @date 2021-03-10
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __CARDSGAME__H__
#define __CARDSGAME__H__

/* Includes: */

#include <stddef.h> /* size_t */

/* Defines: */

typedef struct CardsGame CardsGame;


/*-------------------------------------- Main API Functions: ---------------------------------------*/


/**
 * @brief Creates and returns a pointer to a new CardsGame
 * 
 * @param _numberOfRealPlayers: The number of the Real Players in the CardsGame, must be at least 1
 * @param _numberOfComputerPlayers: The number of the Computer Players in the CardsGame, must be at least 1 if there is only 1 Real Player in the CardsGame
 * @param _endGameScoreCondition: The game ending points condition, to check if the CardsGame should finish (negative value is also valid)
 * 
 * @return [CardsGame*] A pointer to a new CardsGame, else NULL if error: ALLOCATION_ERROR (failed to allocate memory), WRONG_PLAYERS_COUNT
 */
CardsGame* CreateCardsGame(size_t _numberOfRealPlayers, size_t _numberOfComputerPlayers, int _endGameScoreCondition);


/**
 * @brief Destroys a given CardsGame pointer and puts NULL in it, prevents double DestroyCardsGame attempts (to avoid double free)
 * 
 * @param _game: A memory address of a pointer to a CardsGame to destroy and release its memory
 */
void DestroyCardsGame(CardsGame** _game);


/**
 * @brief Runs a given CardsGame's engine
 * 
 * @return [CardsGame*] A pointer to a CardsGame to run
 * 
 * @return [int] 1 if succeed, else 0 if error: NULL_PTR_ERROR (a given pointer is NULL)
 */
int RunCardsGameEngine(CardsGame* _game);


/*--------------------------------- End of Main API Functions -------------------------------------*/


#endif  /* #ifndef __CARDSGAME__H__ */