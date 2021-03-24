/**
 * @file CardsGamePlayer.c
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief CardsGamePlayer module source file
 * @version 0.1
 * @date 2021-03-10
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Includes: */

#include <stdlib.h> /* malloc, free, size_t */
#include <string.h> /* strncpy */
#include "../../Modules/Headers/CardsGamePlayer.h"
#include "../../Modules/Headers/Hand.h"
#include "../../Modules/Headers/Card.h"

/* Define: */

#define MAGIC_NUM 74532811
#define MAX_STRING_LENGTH 100

struct CardsGamePlayer
{
    char* m_playerName;
    int m_isRealPlayer;
    int m_playerScore;
    Hand* m_playerHand;
    int m_magicNumber;
};


/* Validation Functions Declarations: */

static int ValidateCreateCardsGamePlayerParams(const char* _playerName);

/* Helper Functions Declarations: */

static CardsGamePlayer* InitializeNewCardsGamePlayer(CardsGamePlayer** _player, const char* _playerName, int _isRealPlayer);

/*-------------------------------------- Main API Functions: ---------------------------------------*/


CardsGamePlayer* CreateCardsGamePlayer(const char* _playerName, int _isRealPlayer)
{
    CardsGamePlayer* newPlayer = NULL;

    if(!ValidateCreateCardsGamePlayerParams(_playerName))
    {
        return NULL;
    }

    newPlayer = malloc(sizeof(CardsGamePlayer));

    return InitializeNewCardsGamePlayer(&newPlayer, _playerName, _isRealPlayer);
}


void DestroyCardsGamePlayer(CardsGamePlayer** _player)
{
    if(_player && *_player && (*_player)->m_magicNumber == MAGIC_NUM)
    {
        if(((*_player)->m_playerName))
        {
            free((*_player)->m_playerName);
        }

        if(((*_player)->m_playerHand))
        {
            DestroyHand(&((*_player)->m_playerHand));
        }

        (*_player)->m_magicNumber = 0;
        free(*_player);
        *_player = NULL;
    }
}


int ResetCardsGamePlayerHand(CardsGamePlayer* _player, size_t _maxCardsInHand)
{
    if(_player->m_playerHand)
    {
        DestroyHand(&(_player->m_playerHand)); /* Destroy the old Hand */
    }

    _player->m_playerHand = CreateHand(_maxCardsInHand);
    if(!_player->m_playerHand)
    {
        return 0;
    }

    return 1;
}


Card* GetSingleCardFromCardsGamePlayer(CardsGamePlayer* _player, SingleCardStrategyFunction _strategy, Rank _rank, Suite _suite)
{
    if(!_player || !_strategy)
    {
        return NULL;
    }

    return _strategy(_player->m_playerHand, _rank, _suite);
}


Card** GetMultipleCardsFromCardsGamePlayer(CardsGamePlayer* _player, MultipleCardsStrategyFunction _strategy, Rank _rank, Suite _suite)
{
    if(!_player || !_strategy)
    {
        return NULL;
    }

    return _strategy(_player->m_playerHand, _rank, _suite);
}


int GiveSingleCardToCardsGamePlayer(CardsGamePlayer* _player, Card* _cardToGive)
{
    int status = 0;

    if(!_player || !_cardToGive)
    {
        return 0;
    }

    status = TakeCardToHoldInHand(_player->m_playerHand, _cardToGive);
    SortHandByRanksAndSuites(_player->m_playerHand);

    return status;
}


const char* GetNameOfCardsGamePlayer(CardsGamePlayer* _player)
{
    if(!_player)
    {
        return NULL;
    }

    return _player->m_playerName;
}


int SetScoreOfCardsGamePlayer(CardsGamePlayer* _player, int score)
{
    if(!_player)
    {
        return 0;
    }

    _player->m_playerScore = score;

    return 1;
}


int GetScoreOfCardsGamePlayer(CardsGamePlayer* _player)
{
    if(!_player)
    {
        return -1;
    }

    return _player->m_playerScore;
}


int IsRealPlayer(CardsGamePlayer* _player)
{
    if(!_player)
    {
        return -1;
    }

    return _player->m_isRealPlayer;
}


int IsPlayerHoldSpecificCard(CardsGamePlayer* _player, Rank _rank, Suite _suite)
{
    if(!_player)
    {
        return -1;
    }

    return IsCardInHand(_player->m_playerHand, _rank, _suite);
}


size_t GetNumberOfCardsPlayerIsHolding(CardsGamePlayer* _player)
{
    if(!_player)
    {
        return -1;
    }

    return GetNumberOfCardsInHand(_player->m_playerHand);
}


/*--------------------------------- End of Main API Functions -------------------------------------*/


/* Validation Functions: */

static int ValidateCreateCardsGamePlayerParams(const char* _playerName)
{
    return (_playerName) ? 1 : 0;
}


/* Helper Functions: */

static CardsGamePlayer* InitializeNewCardsGamePlayer(CardsGamePlayer** _player, const char* _playerName, int _isRealPlayer)
{
    (*_player)->m_playerName = malloc(MAX_STRING_LENGTH);
    if(!(*_player)->m_playerName)
    {
        DestroyCardsGamePlayer(&(*_player));
        return NULL;
    }

    strncpy((*_player)->m_playerName, _playerName, MAX_STRING_LENGTH);

    (*_player)->m_playerHand = NULL; /* Lazy Hand initialization - when needed (ResetCardsGamePlayerHand function) */

    if(_isRealPlayer)
    {
        (*_player)->m_isRealPlayer = 1; /* True */
    }
    else /* A Computer Player */
    {
        (*_player)->m_isRealPlayer = 0; /* False */
    }

    (*_player)->m_playerScore = 0;
    (*_player)->m_magicNumber = MAGIC_NUM;

    return (*_player);
}
