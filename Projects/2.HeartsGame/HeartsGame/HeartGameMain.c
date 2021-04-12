#include "./Modules/Headers/CardsGame.h"

#define CPU_COUNT 3
#define REAL_PLAYER_COUNT 1
#define END_GAME_SCORE 100

int main(void)
{
    CardsGame* game = CreateCardsGame(REAL_PLAYER_COUNT, CPU_COUNT, END_GAME_SCORE);
    RunCardsGameEngine(game);
    DestroyCardsGame(&game);

    return 0;
}