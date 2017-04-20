#ifndef __InputSystemManager_h_
#define __InputSystemManager_h_

#include "GameState.h"
#include "GuiManager.h"

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <CEGUI/CEGUI.h>

#include <OgreVector3.h>
#include <OgreSceneNode.h>
#include <OgreFrameListener.h>

/*
TODO: consider splitting this class. 
Create separate classes for OIS management, camera management,
and classes for other group of input processing managers.
*/
class InputSystemManager : 
    public OIS::KeyListener,
    public OIS::MouseListener
{
public:
    InputSystemManager(GameState &gameState, GuiManager &guiManager, Ogre::SceneNode *cameraNode);

    void init(size_t windowHnd);
    void setMouseArea(unsigned int width, unsigned int height);
    void destroy();

    bool frameRenderingQueued(const Ogre::FrameEvent& evt);
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

    Ogre::SceneNode *_cameraNode;
    Ogre::Vector3 _cameraMovementDirection;
    const Ogre::Real CAMERA_ROT = 0.13;
    const Ogre::Real CAMERA_MOV_SPEED = 250;

    OIS::InputManager* mInputManager = nullptr;
    OIS::Mouse* mMouse = nullptr;
    OIS::Keyboard* mKeyboard = nullptr;

    CEGUI::MouseButton OIStoCEGUIMouseButton(const OIS::MouseButtonID &buttonID);
};

#endif // #ifndef __InputSystemManager_h_