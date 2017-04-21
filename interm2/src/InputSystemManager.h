#ifndef __InputSystemManager_h_
#define __InputSystemManager_h_

#include "GameState.h"
#include "InputEventListener.h"
#include "GuiManager.h"

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <CEGUI/CEGUI.h>

#include <OgreVector3.h>
#include <OgreSceneNode.h>
#include <OgreCamera.h>
#include <OgreFrameListener.h>

#include <vector>

using std::vector;

class InputSystemManager : 
    public OIS::KeyListener,
    public OIS::MouseListener
{
public:
    InputSystemManager(GameState &gameState);

    void init(size_t windowHnd);
    void setMouseArea(unsigned int width, unsigned int height);
    void destroy();

    bool frameRenderingQueued(const Ogre::FrameEvent& evt);
    void addInputEventListener(InputEventListener *l) { _listeners.push_back(l); };
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

    OIS::InputManager* mInputManager = nullptr;
    OIS::Mouse* mMouse = nullptr;
    OIS::Keyboard* mKeyboard = nullptr;

    vector<InputEventListener*> _listeners;
};

#endif // #ifndef __InputSystemManager_h_