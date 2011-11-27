#include "game.h"

static GameState state;

GameState& gameState() {
    return state;
}

static int score = 0;
int& getScore() {
    return score;
}

void addScore(int level) {
    if (level == 1) {
        level += 10;
    } else if (level == 2) {
        level += 20;
    } else if (level == 3) {
        level += 50;
    } 
}
