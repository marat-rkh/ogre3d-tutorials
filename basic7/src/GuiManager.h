#ifndef __GuiManager_h_
#define __GuiManager_h_

#include <atomic>

class GuiManager {
public:
    GuiManager();
    
    void pressExitButton();
    bool isExitButtonPressed();
private:
    std::atomic<bool> mExitButtonPressed;
};

#endif // #ifndef __GuiManager_h_