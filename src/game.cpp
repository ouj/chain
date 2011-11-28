#include "game.h"

static GameState state;
bool lostHand = false;

bool isLostHand() { return lostHand; }
void setLostHand(bool l) { lostHand = l; }

GameState& gameState() {
    return state;
}

static int score = 0;
int& getScore() {
    return score;
}

void addScore(int level) {
    if (level == 1) {
        score += 10;
    } else if (level == 2) {
        score += 20;
    } else if (level == 3) {
        score += 50;
    } 
}

void reduceScore() {
    score -= 100;
}
