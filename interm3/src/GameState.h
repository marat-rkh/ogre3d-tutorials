#ifndef __GameState_h_
#define __GameState_h_

/*
TODO: consider thread safety issues
*/ 
class GameState {
public:
    GameState();

    bool isExitGame();
    void isExitGame(bool value);
private:
    bool _isExitGame;
};

#endif // #ifndef __GameState_h_