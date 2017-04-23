#ifndef __GameState_h_
#define __GameState_h_

/*
TODO: consider thread safety issues
*/ 
class GameState {
public:
    GameState();

    enum class Mode {
        OBJECTS_CREATION,
        BOX_SELECTION
    };

    bool isExitGame();
    void isExitGame(bool value);
    Mode mode() { return _mode; }
    void mode(Mode mode) { _mode = mode; }
private:
    bool _isExitGame;
    Mode _mode = Mode::OBJECTS_CREATION;
};

#endif // #ifndef __GameState_h_