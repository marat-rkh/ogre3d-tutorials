#include "GameState.h"

GameState::GameState() :
    _isExitGame(false)
{}

bool GameState::isExitGame() { 
    return _isExitGame; 
}

void GameState::isExitGame(bool value) { 
    _isExitGame = value; 
}