/**
 * @file CardsGame.c
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief CardsGame module source file
 * @version 0.1
 * @date 2021-03-11
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Includes: */

#include <stdio.h> /* sprintf */
#include <stdlib.h> /* malloc, free, size_t */
#include "../../Modules/Headers/CardsGame.h"
#include "../../Modules/Headers/CardsGamePlayer.h"
#include "../../Modules/Headers/CardsGameRound.h"
#include "../../Modules/Headers/UI.h"

/* Defines: */

#define MAGIC_NUM 33985612
#define MAX_STRING_BUFFER_LENGTH 200
#define IS_REAL_PLAYER 1
#define IS_NOT_REAL_PLAYER 0

struct CardsGame
{
    CardsGamePlayer** m_players;
    size_t m_playersCount;
    int m_endGameScoreCondition;
    int* m_scoreBoard;
    int m_magicNumber;
};


/* Validation Functions Declarations: */

static int ValidateCreateCardsGamePlayerParams(size_t _numberOfRealPlayers, size_t _numberOfComputerPlayers);

/* Helper Functions Declarations: */

static CardsGame* InitializeNewCardsGame(CardsGame** _game, size_t _numberOfRealPlayers, size_t _numberOfComputerPlayers, int _endGameScoreCondition);
static int CheckEndGameScoreCondition(int* _scoreBoard, size_t _scoreBoardSize, int _endGameScoreCondition);
static int CheckPositiveEndGameScoreCondition(int* _scoreBoard, size_t _scoreBoardSize, int _endGameScoreCondition);
static int CheckNegativeEndGameScoreCondition(int* _scoreBoard, size_t _scoreBoardSize, int _endGameScoreCondition);
static int FetchTotalScores(CardsGame* _game);

/*-------------------------------------- Main API Functions: ---------------------------------------*/


CardsGame* CreateCardsGame(size_t _numberOfRealPlayers, size_t _numberOfComputerPlayers, int _endGameScoreCondition)
{
    CardsGame* newGame = NULL;

    if(!ValidateCreateCardsGamePlayerParams(_numberOfRealPlayers, _numberOfComputerPlayers))
    {
        return NULL;
    }

    newGame = malloc(sizeof(CardsGame));
    if(!newGame)
    {
        return NULL;
    }

    return InitializeNewCardsGame(&newGame, _numberOfRealPlayers, _numberOfComputerPlayers, _endGameScoreCondition);
}


void DestroyCardsGame(CardsGame** _game)
{
    size_t i;

    if(_game && *_game && (*_game)->m_magicNumber == MAGIC_NUM)
    {
        if((*_game)->m_scoreBoard)
        {
            free((*_game)->m_scoreBoard);
        }

        if((*_game)->m_players) /* Optionally to be triggered at the initialization stage - a chance that this pointer is not successfully allocated */
        {
            for(i = 0; i < (*_game)->m_playersCount; i++)
            {
                DestroyCardsGamePlayer(&((*_game)->m_players[i]));
            }
            free((*_game)->m_players);
        }

        (*_game)->m_magicNumber = 0;
        free(*_game);
        *_game = NULL;
    }
}


int RunCardsGameEngine(CardsGame* _game)
{
    size_t i, totalRoundsCount = 0;
    CardsGameRound* round = NULL;
    int score;
    char* buffer = malloc(MAX_STRING_BUFFER_LENGTH);
    if(!_game || !buffer)
    {
        return 0;
    }

    round = CreateCardsGameRound(_game->m_players, _game->m_playersCount);

    while(!CheckEndGameScoreCondition(_game->m_scoreBoard, _game->m_playersCount, _game->m_endGameScoreCondition))
    {
        RunCardsGameRoundEngine(round, _game->m_scoreBoard, ++totalRoundsCount);

        for(i = 0; i < _game->m_playersCount; i++)
        {
            score = GetScoreOfCardsGamePlayer(_game->m_players[i]);
            SetScoreOfCardsGamePlayer(_game->m_players[i], (score + _game->m_scoreBoard[i]));
        }

        SendMessageToUI("Press any key and then Enter to continue to the next round...", NULL);
        GetInputFromUI(buffer, MAX_STRING_BUFFER_LENGTH);
    }

    FetchTotalScores(_game);

    free(buffer);
    DestroyCardsGameRound(&round);

    return 1;
}


/*--------------------------------- End of Main API Functions -------------------------------------*/


/* Validation Functions: */

static int ValidateCreateCardsGamePlayerParams(size_t _numberOfRealPlayers, size_t _numberOfComputerPlayers)
{
    if(_numberOfRealPlayers > 0)
    {
        if(_numberOfRealPlayers < 2 && _numberOfComputerPlayers < 1)
        {
            return 0; /* Not enough players in a game */
        }

        return 1;
    }

    return 0;
}


/* Helper Functions: */

static CardsGame* InitializeNewCardsGame(CardsGame** _game, size_t _numberOfRealPlayers, size_t _numberOfComputerPlayers, int _endGameScoreCondition)
{
    size_t i, j;
    CardsGamePlayer* newPlayer = NULL;
    char* buffer = malloc(MAX_STRING_BUFFER_LENGTH);

    (*_game)->m_playersCount = _numberOfRealPlayers + _numberOfComputerPlayers;

    (*_game)->m_scoreBoard = calloc((*_game)->m_playersCount, sizeof(int));
    if(!(*_game)->m_scoreBoard)
    {
        DestroyCardsGame(&(*_game));
        return NULL;
    }

    (*_game)->m_players = malloc(sizeof(CardsGamePlayer*) * (*_game)->m_playersCount);
    if(!(*_game)->m_players)
    {
        DestroyCardsGame(&(*_game));
        return NULL;
    }

    ClearUI();

    for(i = 0; i < _numberOfRealPlayers; i++)
    {
        SendMessageToUI("Enter Real Player Name: ", GREEN);
        GetInputFromUI(buffer, MAX_STRING_BUFFER_LENGTH);

        newPlayer = CreateCardsGamePlayer(buffer, IS_REAL_PLAYER);
        if(!newPlayer)
        {
            DestroyCardsGame(&(*_game));
            return NULL;
        }

        (*_game)->m_players[i] = newPlayer;
    }

    for(j = 0; j < _numberOfComputerPlayers; j++, i++) /* Continue the counting with i index */
    {
        sprintf(buffer, "%s %ld", "Computer Player", j + 1);

        newPlayer = CreateCardsGamePlayer(buffer, IS_NOT_REAL_PLAYER);
        if(!newPlayer)
        {
            DestroyCardsGame(&(*_game));
            return NULL;
        }

        (*_game)->m_players[i] = newPlayer;
    }

    free(buffer);

    (*_game)->m_endGameScoreCondition = _endGameScoreCondition;
    (*_game)->m_magicNumber = MAGIC_NUM;

    return (*_game);
}


static int CheckEndGameScoreCondition(int* _scoreBoard, size_t _scoreBoardSize, int _endGameScoreCondition)
{
    if(_endGameScoreCondition > 0)
    {
        return CheckPositiveEndGameScoreCondition(_scoreBoard, _scoreBoardSize, _endGameScoreCondition);
    }
    else /* Negative score check needed */
    {
        return CheckNegativeEndGameScoreCondition(_scoreBoard, _scoreBoardSize, _endGameScoreCondition);
    }
}


static int CheckPositiveEndGameScoreCondition(int* _scoreBoard, size_t _scoreBoardSize, int _endGameScoreCondition)
{
    size_t i;

    for(i = 0; i < _scoreBoardSize; i++)
    {
        if(_scoreBoard[i] >= _endGameScoreCondition)
        {
            return 1; /* Game reached its end */
        }
    }

    return 0; /* Game did not end yet */
}


static int CheckNegativeEndGameScoreCondition(int* _scoreBoard, size_t _scoreBoardSize, int _endGameScoreCondition)
{
    size_t i;

    for(i = 0; i < _scoreBoardSize; i++)
    {
        if(_scoreBoard[i] <= _endGameScoreCondition)
        {
            return 1; /* Game reached its end */
        }
    }

    return 0; /* Game did not end yet */
}


static int FetchTotalScores(CardsGame* _game)
{
    size_t i;
    /*CardsGamePlayer* player = NULL;*/
    char* buffer = malloc(MAX_STRING_BUFFER_LENGTH);
    if(!buffer)
    {
        return 0;
    }

    /* TODO: implement better representation of the winner and the loser of the game
    player = FindWinnerInCardsGame(_game);
    sprintf(buffer, "%s %s, %s %d", "Winner", GetNameOfCardsGamePlayer(player), "Score", GetScoreOfCardsGamePlayer(player));
    SendMessageToUI(buffer);

    player = FindLoserInCardsGame(_game);
    sprintf(buffer, "%s: %s, %s: %d", "Loser", GetNameOfCardsGamePlayer(player), "Score", GetScoreOfCardsGamePlayer(player));
    SendMessageToUI(buffer);
    */

    for(i = 0; i < _game->m_playersCount; i++)
    {
        sprintf(buffer, "%s - %s: %d", GetNameOfCardsGamePlayer(_game->m_players[i]), "Total Score", GetScoreOfCardsGamePlayer(_game->m_players[i]));
        SendMessageToUI(buffer, NULL);
    }

    free(buffer);

    return 1;
}
