/**
 * @file CardsGameRound.c
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief CardsGameRound module source file
 * @version 0.1
 * @date 2021-03-11
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Includes: */

#include <stdlib.h> /* malloc, free, size_t */
#include <stdio.h> /* sprintf */
#include <string.h> /* strcpy, strcat */
#include "../../Modules/Headers/CardsGameRound.h"
#include "../../Modules/Headers/Card.h"
#include "../../Modules/Headers/Deck.h"
#include "../../Modules/Headers/Hand.h"
#include "../../Modules/Headers/CardsGamePlayer.h"
#include "../../Modules/Headers/UI.h"

/* Defines: */

#define BUFF_SIZE 300
#define SMALL_BUFF_SIZE 25
#define MAGIC_NUM 64783920
#define CARDS_COUNT 52
#define GAME_OPENING_SWITCHING_CARDS_COUNT 3
#define SUITES 4
#define ALLOWED_INPUT_FOR_CARD_NAME 3

struct CardsGameRound
{
    CardsGamePlayer** m_players;
    size_t m_playersCount;
    Card** m_cardsInGameTable;
    size_t m_decksNumber; /* Only 1 deck in "Hearts" game */
    size_t m_tricksToPlay; /* Depends on: (number of decks * CARDS_COUNT (52)) / players count (== 13 when: 4 players and 1 deck (with 52 cards)) */
    int m_magicNumber;
};


/* Validation Functions Declarations: */

static int ValidateCreateCardsGameRoundParams(CardsGamePlayer** _players, size_t _playersCount);

/* Helper Functions Declarations: */

static CardsGameRound* InitializeNewCardsGameRound(CardsGameRound** _round, CardsGamePlayer** _players, size_t _playersCount);
static void DealCards(Deck* _deck, CardsGamePlayer** _players, size_t _playersCount);
static void RotateCardsBetweenPlayers(CardsGamePlayer** _players, size_t _playersCount, Card*** _openingGameCards, size_t _roundNumber);
static void RotateCardsToLeft(CardsGamePlayer** _players, size_t _playersCount, Card*** _openingGameCards);
static void RotateCardsToRight(CardsGamePlayer** _players, size_t _playersCount, Card*** _openingGameCards);
static void RotateCardsForward(CardsGamePlayer** _players, size_t _playersCount, Card*** _openingGameCards);
static void DoNotRotateCards(CardsGamePlayer** _players, size_t _playersCount, Card*** _openingGameCards);
static void ClearSwitchingOpeningGameCardsMove(Card*** _arrayToClear, size_t _sizeOfArray);
static void CalculateAndUpdateTrickScores(Card** _cardsInGameTable, size_t _cardsInTableCount, int* _scoreBoard, size_t _indexOfLoserPlayer);
static size_t PlayTrick(CardsGamePlayer** _players, Card** _cardsInGameTable, size_t _playersCount, int* _scoreBoard, size_t _indexOfStartingPlayer, size_t _currentTrick);
static void ShowTable(Card** _table, size_t _playersCount);
static void ClearTable(Card** _table, size_t _countOfCardsOnTable);
static void ShowCurrentScore(CardsGamePlayer** _players, size_t _playersCount, int* _scoreBoard);
Card* ForceTakingSingleCard(Hand* _hand, Rank _rank, Suite _suite);
Card* TriggerRealPlayerMove(Hand* _hand, Rank _rank, Suite _suite);
Card** TriggerRealPlayerRoundStartingMove(Hand* _hand, Rank _rank, Suite _suite);
Card* TriggerComputerPlayerMove(Hand* _hand, Rank _rank, Suite _suite);
Card** TriggerComputerPlayerRoundStartingMove(Hand* _hand, Rank _rank, Suite _suite);
Card* AskUserToGiveOneCard(Hand* _playerHand);
Rank GetUserInputRank(const char* _userInputStr);
Suite GetUserInputSuite(const char* _userInputStr);


/*-------------------------------------- Main API Functions: ---------------------------------------*/


CardsGameRound* CreateCardsGameRound(CardsGamePlayer** _players, size_t _playersCount)
{
    CardsGameRound* newRound = NULL;

    if(!ValidateCreateCardsGameRoundParams(_players, _playersCount))
    {
        return NULL;
    }

    newRound = malloc(sizeof(CardsGameRound));
    if(!newRound)
    {
        return NULL;
    }

    return InitializeNewCardsGameRound(&newRound, _players, _playersCount);
}


void DestroyCardsGameRound(CardsGameRound** _round)
{
    if(_round && *_round && (*_round)->m_magicNumber == MAGIC_NUM)
    {
        /* Must not free _players array - that array belongs to CardsGame object */

        size_t i;

        if((*_round)->m_cardsInGameTable)
        {
            for(i = 0; i < (*_round)->m_playersCount; i++)
            {
                DestroyCard(&((*_round)->m_cardsInGameTable[i]));
            }

            free((*_round)->m_cardsInGameTable);
        }

        (*_round)->m_magicNumber = 0;
        free(*_round);
        *_round = NULL;
    }
}


int RunCardsGameRoundEngine(CardsGameRound* _round, int* _scoreBoard, size_t _roundNumber)
{
    size_t i, indexOfStartingPlayer, currentTrick;
    Deck* deck = CreateDeck(_round->m_decksNumber);
    Card*** openingGameCards = malloc(_round->m_playersCount * sizeof(Card**));
    char* buffer = malloc(BUFF_SIZE);

    if(!deck || !openingGameCards)
    {
        return 0;
    }

    ClearUI();
    SendMessageToUI("*******************************************************************************", BLUE);
    SendMessageToUI("                                Hearts Cards Game                              ", MAGENTA);
    sprintf(buffer, "                                   Round %ld                                   ", _roundNumber);
    SendMessageToUI(buffer, MAGENTA);
    SendMessageToUI("*******************************************************************************", BLUE);

    ShuffleDeck(deck);
    DealCards(deck, _round->m_players, _round->m_playersCount);

    /* Round Engine - Starting move: */

    for(i = 0; i < _round->m_playersCount; i++)
    {
        if(IsRealPlayer(_round->m_players[i]))
        {
            sprintf(buffer, "------------------------ %s\'s game opening turn: --------------------------", GetNameOfCardsGamePlayer(_round->m_players[i]));
            SendMessageToUI(buffer, NULL);
            openingGameCards[i] = GetMultipleCardsFromCardsGamePlayer(_round->m_players[i], &TriggerRealPlayerRoundStartingMove, NO_RANK, NO_SUITE);
        }
        else /* Is a Computer Player */
        {
            openingGameCards[i] = GetMultipleCardsFromCardsGamePlayer(_round->m_players[i], &TriggerComputerPlayerRoundStartingMove, NO_RANK, NO_SUITE);
        }
    }

    RotateCardsBetweenPlayers(_round->m_players, _round->m_playersCount, openingGameCards, _roundNumber);
    ClearSwitchingOpeningGameCardsMove(openingGameCards, _round->m_playersCount);

    /* Round Engine - Main: */

    for(i = 0; i < _round->m_playersCount; i++) /* First trick's beginner */
    {
        if(IsPlayerHoldSpecificCard(_round->m_players[i], TWO, CLUB))
        {
            indexOfStartingPlayer = i;
            break;
        }
    }

    for(currentTrick = 1; currentTrick <= _round->m_tricksToPlay; currentTrick++)
    {
        sprintf(buffer, "------------------------------ Trick %ld ---------------------------------", currentTrick);
        SendMessageToUI(buffer , GREEN);
        indexOfStartingPlayer = PlayTrick(_round->m_players, _round->m_cardsInGameTable, _round->m_playersCount, _scoreBoard, indexOfStartingPlayer, currentTrick); /* To get the loser of the current trick - he will start next trick*/
        CalculateAndUpdateTrickScores(_round->m_cardsInGameTable, _round->m_playersCount, _scoreBoard, indexOfStartingPlayer); /* The next trick starting player - is the loser of the current trick */
        ClearTable(_round->m_cardsInGameTable, _round->m_playersCount);
        ShowCurrentScore(_round->m_players, _round->m_playersCount, _scoreBoard);

        SendMessageToUI("Press any key and then Enter to continue to the next trick...", NULL);
        GetInputFromUI(buffer, BUFF_SIZE);
    }


    free(buffer);
    DestroyDeck(&deck);

    return 1;
}


/*--------------------------------- End of Main API Functions -------------------------------------*/


/* Validation Functions: */

static int ValidateCreateCardsGameRoundParams(CardsGamePlayer** _players, size_t _playersCount)
{
    return (_players && _playersCount > 1) ? 1 : 0;
}


/* Helper Functions: */

static CardsGameRound* InitializeNewCardsGameRound(CardsGameRound** _round, CardsGamePlayer** _players, size_t _playersCount)
{
    (*_round)->m_players = _players;
    (*_round)->m_playersCount = _playersCount;
    (*_round)->m_cardsInGameTable = malloc(_playersCount * sizeof(Card*)); /* Every trick each player puts only 1 card in "Hearts game" */
    if(!(*_round)->m_cardsInGameTable)
    {
        DestroyCardsGameRound(&(*_round));
    }

    (*_round)->m_decksNumber = 1;
    (*_round)->m_tricksToPlay = ((((*_round)->m_decksNumber) * CARDS_COUNT) / ((*_round)->m_playersCount));
    (*_round)->m_magicNumber = MAGIC_NUM;

    return (*_round);
}


static void DealCards(Deck* _deck, CardsGamePlayer** _players, size_t _playersCount)
{
    size_t i, j;
    Card* card = NULL;
    size_t cardsToDealForEachPlayer = (CARDS_COUNT / _playersCount);

    for(i = 0; i < _playersCount; i++)
    {
        ResetCardsGamePlayerHand(_players[i], cardsToDealForEachPlayer);

        for(j = 0; j < cardsToDealForEachPlayer; j++)
        {
            card = DrawCardFromDeck(_deck);
            GiveSingleCardToCardsGamePlayer(_players[i] , card);
        }
    }
}


static void RotateCardsBetweenPlayers(CardsGamePlayer** _players, size_t _playersCount, Card*** _openingGameCards, size_t _roundNumber)
{
    size_t rotationRuleIndicator = (_roundNumber % 4) + 1;

    if(rotationRuleIndicator == 1)
    {
        RotateCardsToLeft(_players, _playersCount, _openingGameCards);
    }
    else if(rotationRuleIndicator == 2)
    {
        RotateCardsToRight(_players, _playersCount, _openingGameCards);
    }
    else if(rotationRuleIndicator == 3)
    {
        RotateCardsForward(_players, _playersCount, _openingGameCards);
    }
    else /* if(rotationRuleIndicator == 4) */
    {
        DoNotRotateCards(_players, _playersCount, _openingGameCards);
    }
}


static void RotateCardsToLeft(CardsGamePlayer** _players, size_t _playersCount, Card*** _openingGameCards)
{
    size_t i, j;

    /*Special case for the first player (index 0 - handling negative size_t index): */
    for(j = 0; j < GAME_OPENING_SWITCHING_CARDS_COUNT; j++)
    {
        GiveSingleCardToCardsGamePlayer(_players[(_playersCount - 1)], _openingGameCards[0][j]);
    }

    for(i = 1; i < _playersCount; i++)
    {
        for(j = 0; j < GAME_OPENING_SWITCHING_CARDS_COUNT; j++)
        {
            GiveSingleCardToCardsGamePlayer(_players[(i - 1)], _openingGameCards[i][j]);
        }
    }
}


static void RotateCardsToRight(CardsGamePlayer** _players, size_t _playersCount, Card*** _openingGameCards)
{
    size_t i, j;

    for(i = 0; i < _playersCount; i++)
    {
        for(j = 0; j < GAME_OPENING_SWITCHING_CARDS_COUNT; j++)
        {
            GiveSingleCardToCardsGamePlayer(_players[(i + 1) % _playersCount], _openingGameCards[i][j]);
        }
    }
}


static void RotateCardsForward(CardsGamePlayer** _players, size_t _playersCount, Card*** _openingGameCards)
{
    size_t i, j;

    for(i = 0; i < _playersCount; i++)
    {
        for(j = 0; j < GAME_OPENING_SWITCHING_CARDS_COUNT; j++)
        {
            GiveSingleCardToCardsGamePlayer(_players[(i + 2) % _playersCount], _openingGameCards[i][j]);
        }
    }
}


static void DoNotRotateCards(CardsGamePlayer** _players, size_t _playersCount, Card*** _openingGameCards)
{
    size_t i, j;

    for(i = 0; i < _playersCount; i++)
    {
        for(j = 0; j < GAME_OPENING_SWITCHING_CARDS_COUNT; j++)
        {
            GiveSingleCardToCardsGamePlayer(_players[i], _openingGameCards[i][j]);
        }
    }
}


static void ClearSwitchingOpeningGameCardsMove(Card*** _arrayToClear, size_t _sizeOfArray)
{
    /* Clearing the memory without freeing the Card* (cards in game) */

    size_t i;

    for(i = 0; i < _sizeOfArray; i++)
    {
        free(_arrayToClear[i]);
    }

    free(_arrayToClear);
}


static size_t PlayTrick(CardsGamePlayer** _players, Card** _cardsInGameTable, size_t _playersCount, int* _scoreBoard, size_t _indexOfStartingPlayer, size_t _currentTrickNumber)
{
    Suite firstSuiteInTable = NO_SUITE;
    int hasHeartWasPlaced = 0, isValidMove = 0;
    size_t j, i, highestCardIndex, currentCardsCountOnTable = 0, currentPlayerTurn = _indexOfStartingPlayer; /* In first turn - should be the index of the player with the "Two Club" */
    Card* card = NULL;
    char* buff = malloc(BUFF_SIZE);
    if(!buff)
    {
        return 0;
    }

    if(_currentTrickNumber == 1) /* Starting with "Two Club" */
    {
        card = GetSingleCardFromCardsGamePlayer(_players[currentPlayerTurn], &ForceTakingSingleCard, TWO, CLUB);
        _cardsInGameTable[_indexOfStartingPlayer] = card;
        currentCardsCountOnTable++;
        currentPlayerTurn = (currentPlayerTurn + 1) % _playersCount;
        firstSuiteInTable = CLUB;
        ShowTable(_cardsInGameTable, _playersCount);
    }

    while(currentCardsCountOnTable < _playersCount)
    {
        if(IsRealPlayer(_players[currentPlayerTurn]))
        {
            while(!isValidMove) /* TODO: Possible upgrade - extract this validation code to other function */
            {
                card = GetSingleCardFromCardsGamePlayer(_players[currentPlayerTurn], &TriggerRealPlayerMove, NO_RANK, NO_SUITE);

                if(firstSuiteInTable != NO_SUITE) /* If so, should place a card from the current suite, excluding a case he is missing a card of that suite */
                {
                    if(GetCardSuite(card) != firstSuiteInTable)
                    {
                        /* Check if player is not holding the current suite */

                        if(!IsPlayerHoldSpecificCard(_players[currentPlayerTurn], NO_RANK, firstSuiteInTable))
                        {
                            if(GetCardSuite(card) == HEART)
                            {
                                if(hasHeartWasPlaced)
                                {
                                    isValidMove = 1;
                                }
                                else
                                {
                                    /* Check if player is not holding other suite then Hearts */

                                    if(!IsPlayerHoldSpecificCard(_players[currentPlayerTurn], NO_RANK, CLUB)
                                    || !IsPlayerHoldSpecificCard(_players[currentPlayerTurn], NO_RANK, DIAMOND)
                                    || !IsPlayerHoldSpecificCard(_players[currentPlayerTurn], NO_RANK, SPADE))
                                    {
                                        isValidMove = 1; /* Player doesn't hold other suite then Heart - must be able to use it */
                                    }
                                    else
                                    {
                                        isValidMove = 0;
                                        SendErrorToUI("Wrong card selection - please do not try to break the rules, you should select other suite then Heart...");
                                        GiveSingleCardToCardsGamePlayer(_players[currentPlayerTurn], card); /* Return the card to its owner */
                                    }
                                }
                            }
                            else /* The selected card is not Heart */     /* Known issue: selected card could be "Queen Spade" */
                            {
                                isValidMove = 1;
                            }
                        }
                        else /* Player is holding the current suite, and he did not select it */
                        {
                            isValidMove = 0;
                            SendErrorToUI("Wrong card selection - please do not try to break the rules - you should select a card from the same suite as the first card...");
                            GiveSingleCardToCardsGamePlayer(_players[currentPlayerTurn], card); /* Return the card to its owner */
                        }
                    }
                    else /* Selected the current suite in the table */
                    {
                        isValidMove = 1;
                    }
                }
                else /* No cards on table (no current suite) */
                {
                    if(GetCardSuite(card) == HEART && !hasHeartWasPlaced)
                    {
                        /* Check if player is not holding other suite then Hearts */

                        if(!IsPlayerHoldSpecificCard(_players[currentPlayerTurn], NO_RANK, CLUB)
                        || !IsPlayerHoldSpecificCard(_players[currentPlayerTurn], NO_RANK, DIAMOND)
                        || !IsPlayerHoldSpecificCard(_players[currentPlayerTurn], NO_RANK, SPADE)) /* Known issue: That will force the player to use his "Queen Spade" (if holds this card) before using his Hearts */
                        {
                            isValidMove = 1; /* Player doesn't hold other suite then Heart - must be able to use it */
                        }
                        else
                        {
                            isValidMove = 0;
                            SendErrorToUI("Wrong card selection - please do not try to break the rules, you should select other suite then Heart...");
                            GiveSingleCardToCardsGamePlayer(_players[currentPlayerTurn], card); /* Return the card to its owner */
                        }
                    }
                    else /* Card is not Heart and this is the first suite in the table */
                    {
                        isValidMove = 1;
                    }
                }
            }
        }
        else /* Is a Computer Player */
        {
            card = GetSingleCardFromCardsGamePlayer(_players[currentPlayerTurn], &TriggerComputerPlayerMove, NO_RANK, firstSuiteInTable);
        }

        if(firstSuiteInTable == NO_SUITE)
        {
            firstSuiteInTable = GetCardSuite(card);
        }

        if(GetCardSuite(card) == HEART)
        {
            hasHeartWasPlaced = 1;
        }

        _cardsInGameTable[currentPlayerTurn] = card;
        currentPlayerTurn = (currentPlayerTurn + 1) % _playersCount;
        currentCardsCountOnTable++;

        ShowTable(_cardsInGameTable, _playersCount);
    }


    highestCardIndex = _indexOfStartingPlayer;
    for(j = 1, i = (_indexOfStartingPlayer + 1) % _playersCount; j < _playersCount; j++, i = (i + 1) % _playersCount)
    {
        if(GetCardSuite(_cardsInGameTable[i]) == firstSuiteInTable)
        {
            if(GetCardValue(_cardsInGameTable[highestCardIndex]) < GetCardValue(_cardsInGameTable[i]))
            {
                highestCardIndex = i;
            }
        }
    }

    sprintf(buff, "%s %ld %s: %s", "Trick", _currentTrickNumber, "Loser", GetNameOfCardsGamePlayer(_players[highestCardIndex]));
    SendMessageToUI(buff, BOLD_ON);

    free(buff);

    return highestCardIndex;
}


static void CalculateAndUpdateTrickScores(Card** _cardsInGameTable, size_t _cardsInTableCount, int* _scoreBoard, size_t _indexOfLoserPlayer)
{
    size_t i;
    int totalTrickScore = 0;

    for(i = 0; i < _cardsInTableCount; i++)
    {
        if(GetCardSuite(_cardsInGameTable[i]) == HEART)
        {
            totalTrickScore += 1;
        }

        if(GetCardSuite(_cardsInGameTable[i]) == SPADE && GetCardRank(_cardsInGameTable[i]) == QUEEN)
        {
            totalTrickScore += 13;
        }
    }
    /* TODO: Implement Hearts' "Shoot The Moon" rule */
    _scoreBoard[_indexOfLoserPlayer] += totalTrickScore;
}


static void ShowTable(Card** _table, size_t _playersCount)
{
    size_t i;
    char* buffer = malloc(BUFF_SIZE);
    char* tempBuf = malloc(SMALL_BUFF_SIZE);
    if(!buffer || !tempBuf)
    {
        return;
    }


    SendMessageToUI("Game Table:", CYAN);

    for(i = 0; i < _playersCount; i++)
    {
        if(!_table[i])
        {
            if(i == 0)
            {
                strcpy(buffer, "[ ]\t");
            }
            else
            {
                strcat(buffer, "[ ]\t");
            }
        }
        else
        {
            if(i == 0)
            {
                sprintf(tempBuf, "%s\t", GetCardName(_table[i]));
                strcpy(buffer, tempBuf);
            }
            else
            {
                sprintf(tempBuf, "%s\t", GetCardName(_table[i]));
                strcat(buffer, tempBuf);
            }
        }
    }

    SendMessageToUI(buffer, NULL);

    free(buffer);
    free(tempBuf);
}

/* Older version ShowTable:
static void ShowTable(Card** _table, size_t _playersCount)
{
    size_t i;

    SendMessageToUI("Game Table:", CYAN);

    for(i = 0; i < _playersCount; i++)
    {
        if(!_table[i])
        {
            SendMessageToUI("[ ]", NULL);
        }
        else
        {
            SendMessageToUI(GetCardName(_table[i]), NULL);
        }
    }
}
*/


static void ClearTable(Card** _table, size_t _countOfCardsOnTable)
{
    size_t i;

    for(i = 0; i < _countOfCardsOnTable; i++)
    {
        DestroyCard(&(_table[i]));
    }
}


static void ShowCurrentScore(CardsGamePlayer** _players, size_t _playersCount, int* _scoreBoard)
{
    size_t i;
    char* buf = malloc(BUFF_SIZE);
    if(!buf)
    {
        return;
    }

    for(i = 0; i < _playersCount; i++)
    {
        sprintf(buf, "%s - %s: %d", GetNameOfCardsGamePlayer(_players[i]), "Current Score", _scoreBoard[i]);
        SendMessageToUI(buf, NULL);
    }

    free(buf);
}


/*----------------------------------- Strategy Functions Area: ----------------------------------*/

/***************************************************************************************************
Note: the strategy functions for the real players was created to make a complete separation
between the logic units (each class in the code), and the UI unit.
That way, the gateway of the program's units is the CardsGameRound unit - that should be (in my mind)
the middle connection part between all the other units.
The logic units know how to have a string representation of themselves (char* is a legal data type
of-course), and the CardsGameRound unit (as a main connection unit between them - sends the string to
the UI (that knows ONLY string type).
Now, we have a partition - like in a real world objects (and we are not allowing "printing" through
the logic classes).
****************************************************************************************************/


Card* ForceTakingSingleCard(Hand* _hand, Rank _rank, Suite _suite)
{
    return GetCardFromHand(_hand, _rank, _suite);
}


Card* TriggerRealPlayerMove(Hand* _hand, Rank _rank, Suite _suite)
{
    Card* card = NULL;

    SendMessageToUI("\nPlease choose one card to place in the game table:", NULL);
    SendNotificationToUI("You can pick a card by giving as an input the name of the card, as presented in the list below\n(the rank should be a character between 2 - 10 or J - A, the suite should be: H/S/D/C)\nOnly 3 characters are allowed as an input!");
    SendMessageToUI(GetAllCardsNamesFromHand(_hand), NULL);

    card = AskUserToGiveOneCard(_hand);
    SortHandByRanksAndSuites(_hand);

    return card;
}


Card** TriggerRealPlayerRoundStartingMove(Hand* _hand, Rank _rank, Suite _suite)
{
    /* Returns an array of 3 Cards */

    size_t i;
    Card** cards = malloc(GAME_OPENING_SWITCHING_CARDS_COUNT * sizeof(Card*));
    char* buffer = malloc(BUFF_SIZE);
    if(!cards || !buffer)
    {
        return NULL;
    }

    SendMessageToUI("\nPlease choose three cards to give to the other players:", NULL);
    SendNotificationToUI("You can pick a card by giving as an input the name of the card, as presented in the list below\n(the rank should be a character between 2 - 10 or J - A, the suite should be: H/S/D/C)\nOnly 3 characters are allowed as an input!");
    SendMessageToUI(GetAllCardsNamesFromHand(_hand), NULL);

    for(i = 0; i < GAME_OPENING_SWITCHING_CARDS_COUNT; i++)
    {
        sprintf(buffer, "------------------------------- Card %ld ----------------------------------", (i + 1));
        SendMessageToUI(buffer , NULL);
        cards[i] = AskUserToGiveOneCard(_hand);
    }

    SortHandByRanksAndSuites(_hand);

    free(buffer);

    return cards;
}


Card* TriggerComputerPlayerMove(Hand* _hand, Rank _rank, Suite _suite)
{
    size_t i;
    Card* card = NULL;
    Suite suites[] = {SPADE, DIAMOND, CLUB, HEART}; /* Heart Suite is the last choice */

    if(_suite == NO_SUITE)
    {
        return GetLowestCardByRank(_hand);
    }

    if((card = GetLowestCardBySuite(_hand, _suite))) /* Tries to pick a Card first from the given suite */
    {
        return card;
    }

    for(i = 0; i < SUITES; i++)
    {
        if(suites[i] != _suite)
        {
            if((card = GetHighestCardBySuite(_hand, suites[i]))) /* Doing that that way (and not just picking the highest rank) - to avoid picking a Heart Suite Card, but still giving away a high valued Card from the hand - while the suite is not the current first suite on the table */
            {
                return card;
            }
        }
    }

    return NULL; /* Failed to get a card */
}

Card** TriggerComputerPlayerRoundStartingMove(Hand* _hand, Rank _rank, Suite _suite)
{
    /* Returns an array of 3 Cards */

    size_t i;
    Card** cards = malloc(GAME_OPENING_SWITCHING_CARDS_COUNT * sizeof(Card*));
    if(!cards)
    {
        return NULL;
    }

    for(i = 0; i < GAME_OPENING_SWITCHING_CARDS_COUNT; i++)
    {
        cards[i] = GetHighestCardByRank(_hand); /* gives away the most 3 high ranked Cards from the hand */
    }

    return cards;
}


Card* AskUserToGiveOneCard(Hand* _playerHand)
{
    Card* userCard = NULL;
    Rank userInputRank = WRONG_RANK;
    Suite userInputSuite = WRONG_SUITE;
    char* userInputStr = malloc(ALLOWED_INPUT_FOR_CARD_NAME + 2); /* adding +2 because of the \n (fgets) and \0 */
    if(!userInputStr)
    {
        return NULL;
    }

    while(!userCard)
    {
        GetInputFromUI(userInputStr, ALLOWED_INPUT_FOR_CARD_NAME + 1); /* +1 for '\0' */

        userInputRank = GetUserInputRank(userInputStr);
        userInputSuite = GetUserInputSuite(userInputStr);

        if(userInputRank == WRONG_RANK || userInputSuite == WRONG_SUITE)
        {
            SendErrorToUI("Wrong input - please enter correct input...");
            /* userCard is still NULL */
        }
        else
        {
            SendNotificationToUI("You have chosen:");
            SendNotificationToUI(userInputStr);

            userCard = GetCardFromHand(_playerHand, userInputRank, userInputSuite);

            if(!userCard)
            {
                SendErrorToUI("Wrong card selection - please enter a card that you own...");
            }
        }
    }

    free(userInputStr);

    return userCard;
}


Rank GetUserInputRank(const char* _userInputStr)
{
    Rank rank = WRONG_RANK;

    if(_userInputStr[0] == '1' && _userInputStr[1] == '0') /* Special case for "10" - 2 characters rank */
    {
        return TEN;
    }


    switch (_userInputStr[0])
    {
    case '2':
        rank = TWO;
        break;
    case '3':
        rank = THREE;
        break;
    case '4':
        rank = FOUR;
        break;
    case '5':
        rank = FIVE;
        break;
    case '6':
        rank = SIX;
        break;
    case '7':
        rank = SEVEN;
        break;
    case '8':
        rank = EIGHT;
        break;
    case '9':
        rank = NINE;
        break;
    case 'J':
        rank = JACK;
        break;
    case 'Q':
        rank = QUEEN;
        break;
    case 'K':
        rank = KING;
        break;
    case 'A':
        rank = ACE;
        break;
    default:
        rank = WRONG_RANK;
        break;
    }

    return rank;
}


Suite GetUserInputSuite(const char* _userInputStr)
{
    Suite suite = WRONG_SUITE;
    char charToLookAt = (_userInputStr[1] == '0') ? _userInputStr[2] : _userInputStr[1]; /* Special case for "10" - 2 characters rank */

    switch (charToLookAt)
    {
    case 'H':
        suite = HEART;
        break;
    case 'S':
        suite = SPADE;
        break;
    case 'D':
        suite = DIAMOND;
        break;
    case 'C':
        suite = CLUB;
        break;
    default:
        suite = WRONG_SUITE;
        break;
    }

    return suite;
}


