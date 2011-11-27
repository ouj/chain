#ifndef _GAME_H_
#define _GAME_H_

enum GameState {
    GS_INIT,
    GS_CALI,
    GS_LOST,
    GS_BEGIN,
    GS_RUNNING,
    GS_STOP
};

GameState& gameState();
int& getScore();
void addScore(int level);

#endif // _GAME_H_