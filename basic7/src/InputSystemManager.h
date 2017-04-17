#ifndef __InputSystemManager_h_
#define __InputSystemManager_h_

#include "GameState.h"
#include "GuiManager.h"

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <CEGUI/CEGUI.h>

class InputSystemManager : 
    public OIS::KeyListener,
    public OIS::MouseListener
{
public:
    InputSystemManager(GameState &gameState, GuiManager &guiManager);

    void init(size_t windowHnd);
    void setMouseArea(unsigned int width, unsigned int height);
    void destroy();
    void notifyFrameRenderingQueued();
protected:
    // OIS::KeyListener
    bool keyPressed(const OIS::KeyEvent &arg) override;
    bool keyReleased(const OIS::KeyEvent &arg) override;
    // OIS::MouseListener
    bool mouseMoved(const OIS::MouseEvent &arg) override;
    bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) override;
    bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) override;
private:
    GameState &_gameState;
    GuiManager &_GUIManager;

    OIS::InputManager* mInputManager = nullptr;
    OIS::Mouse* mMouse = nullptr;
    OIS::Keyboard* mKeyboard = nullptr;

    CEGUI::MouseButton OIStoCEGUIMouseButton(const OIS::MouseButtonID &buttonID);
};

#endif // #ifndef __InputSystemManager_h_