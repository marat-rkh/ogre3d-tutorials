/*
-----------------------------------------------------------------------------
Filename:    OgreApplication.h
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/
Tutorial Framework (for Ogre 1.9)
http://www.ogre3d.org/wiki/
-----------------------------------------------------------------------------
*/

#ifndef __OgreApplication_h_
#define __OgreApplication_h_

#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreWindowEventUtilities.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>

#include "GameState.h"
#include "GuiManager.h"

class OgreApplication : 
    public Ogre::WindowEventListener, 
    public Ogre::FrameListener,
    public OIS::KeyListener,
    public OIS::MouseListener
{
public:
    OgreApplication();
    virtual ~OgreApplication();

    bool go();
protected:
    // Ogre::WindowEventListener
    void windowResized(Ogre::RenderWindow* rw) override;
    void windowClosed(Ogre::RenderWindow* rw) override;
    // Ogre::FrameListener
    bool frameRenderingQueued(const Ogre::FrameEvent& evt) override;
    // OIS::KeyListener
    bool keyPressed(const OIS::KeyEvent &arg) override;
    bool keyReleased(const OIS::KeyEvent &arg) override;
    // OIS::MouseListener
    bool mouseMoved(const OIS::MouseEvent &arg) override;
    bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) override;
    bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) override;
private:
    Ogre::Root* mRoot = nullptr;
    Ogre::String mResourcesCfg = Ogre::StringUtil::BLANK;
    Ogre::String mPluginsCfg = Ogre::StringUtil::BLANK;
    Ogre::RenderWindow* mWindow = nullptr;
    Ogre::SceneManager* mSceneMgr = nullptr;

    Ogre::Camera* mCamera = nullptr;

    OIS::InputManager* mInputManager = nullptr;
    OIS::Mouse* mMouse = nullptr;
    OIS::Keyboard* mKeyboard = nullptr;

    GameState _gameState;

    GuiManager _GUIManager;

    bool initOgre();
    void addResourceLocations(const Ogre::String &resourcesCfg);
    void initOIS();
    void setOISMouseArea(Ogre::RenderWindow* rw);
    void destroyOIS(Ogre::RenderWindow* rw);
    CEGUI::MouseButton OIStoCEGUIMouseButton(const OIS::MouseButtonID &buttonID);
    void createScene();

    void ogreLog(const Ogre::String &msg);
};

#endif // #ifndef __OgreApplication_h_