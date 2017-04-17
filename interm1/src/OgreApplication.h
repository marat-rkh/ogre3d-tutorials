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

#include "GameState.h"
#include "InputSystemManager.h"
#include "GuiManager.h"

#include <deque>

class OgreApplication : 
    public Ogre::WindowEventListener, 
    public Ogre::FrameListener
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
private:
    Ogre::Root* mRoot = nullptr;
    Ogre::String mResourcesCfg = Ogre::StringUtil::BLANK;
    Ogre::String mPluginsCfg = Ogre::StringUtil::BLANK;
    Ogre::RenderWindow* mWindow = nullptr;
    Ogre::SceneManager* mSceneMgr = nullptr;

    Ogre::Camera* mCamera = nullptr;

    GameState _gameState;

    InputSystemManager _inputSystemManager;
    GuiManager _GUIManager;

    std::deque<Ogre::Vector3> mWalkList;
 
    Ogre::Real mDistance;
    Ogre::Real mWalkSpd;
    Ogre::Vector3 mDirection;
    Ogre::Vector3 mDestination;
    Ogre::AnimationState* mAnimationState;
    Ogre::Entity* mEntity;
    Ogre::SceneNode* mNode;

    bool initOgre();
    void addResourceLocations(const Ogre::String &resourcesCfg);
    void createScene();

    void ogreLog(const Ogre::String &msg);

    bool nextLocation();
};

#endif // #ifndef __OgreApplication_h_