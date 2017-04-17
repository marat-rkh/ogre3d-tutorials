#ifndef __GameState_h_
#define __GameState_h_

#include <atomic>

class GameState {
public:
    GameState();

    bool isExitGame();
    void isExitGame(bool value);
private:
    std::atomic<bool> _isExitGame;
};

#endif // #ifndef __GameState_h_