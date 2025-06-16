#ifndef ONLINE_MODE_CALL
#include "gamemenu.h"
#define ONLINE_MODE_CALL
#endif


typedef enum endgame_signals
{
    PLAYER_WIN = 100,
    PLAYER_LOSE,
    PLAYER_BUST,
    PLAYER_CHARLIE,
    PLAYER_BLACKJACK,
    PLAYER_DRAW
} SIGNAL;

int GameStart(ONMODE online_mode);