/*
-----------------------------------------------------------------------------
Filename:    OgreApplication.cpp
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

#include "OgreApplication.h"
 
#include <OgreException.h>
#include <OgreConfigFile.h>
#include <OgreRenderWindow.h>

#include <OgreEntity.h>
 
OgreApplication::OgreApplication() :
    _GUIManager(_gameState),
    _inputSystemManager(_gameState, _GUIManager),
    mDistance(0),
    mWalkSpd(70.0),
    mDirection(Ogre::Vector3::ZERO),
    mDestination(Ogre::Vector3::ZERO),
    mAnimationState(0),
    mEntity(0),
    mNode(0)
{}

OgreApplication::~OgreApplication() {
    Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
    windowClosed(mWindow);
    delete mRoot;
}

bool OgreApplication::go() {
    if(!initOgre()) {
        return false;
    }
    ogreLog("*** Initializing OIS ***");
    size_t windowHnd = 0;
    mWindow->getCustomAttribute("WINDOW", &windowHnd);
    _inputSystemManager.init(windowHnd);
    windowResized(mWindow);
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);
    
    ogreLog("*** Initializing GEGUI ***");
    _GUIManager.initGUISystem();
    
    createScene();
    _GUIManager.setupGUI();
    mRoot->addFrameListener(this);
    mRoot->startRendering();
    return true;
}

bool OgreApplication::initOgre() {
#ifdef _DEBUG
    mResourcesCfg = "resources_d.cfg";
    mPluginsCfg = "plugins_d.cfg";
#else
    mResourcesCfg = "resources.cfg";
    mPluginsCfg = "plugins.cfg";
#endif

    mRoot = new Ogre::Root(mPluginsCfg);
    addResourceLocations(mResourcesCfg);
    if(!(mRoot->restoreConfig() || mRoot->showConfigDialog())) {
        // TODO remove ogre.cfg, it probably contains invalid settings
        // TODO throw an exception instead of returning `false`
        return false;
    }
    mWindow = mRoot->initialise(true, "OgreApplication Render Window");

    // Note: TextureManager is initialized AFTER the render system is created
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);

    mCamera = mSceneMgr->createCamera("MainCam");
    mCamera->setPosition(0, 0, 80);
    mCamera->lookAt(0, 0, -300);
    mCamera->setNearClipDistance(5);

    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));
    mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

    return true;
}

void OgreApplication::addResourceLocations(const Ogre::String &resourcesCfg) {
    Ogre::ConfigFile cf;
    cf.load(resourcesCfg);

    Ogre::String section, name, locType;
    Ogre::ConfigFile::SectionIterator secIt = cf.getSectionIterator();

    while (secIt.hasMoreElements()) {
        section = secIt.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap* settings = secIt.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator it;
        for (it = settings->begin(); it != settings->end(); ++it) {
            locType = it->first;
            name = it->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(name, locType, section);
        }
    }
}

void OgreApplication::createScene() {
    mSceneMgr->setAmbientLight(Ogre::ColourValue(1, 1, 1));

    mEntity = mSceneMgr->createEntity("robot.mesh");
    mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(0, 0, 25.0));
    mNode->attachObject(mEntity);

    mWalkList.push_back(Ogre::Vector3(550.0, 0, 50.0));
    mWalkList.push_back(Ogre::Vector3(-100.0, 0, -200.0));
    mWalkList.push_back(Ogre::Vector3(0, 0, 25.0));

    Ogre::Entity* ent;
    Ogre::SceneNode* node;
     
    ent = mSceneMgr->createEntity("knot.mesh");
    node = mSceneMgr->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(0, -10.0, 25.0));
    node->attachObject(ent);
    node->setScale(0.1, 0.1, 0.1);

    ent = mSceneMgr->createEntity("knot.mesh");
    node = mSceneMgr->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(550.0, -10.0, 50.0));
    node->attachObject(ent);
    node->setScale(0.1, 0.1, 0.1);
     
    ent = mSceneMgr->createEntity("knot.mesh");
    node = mSceneMgr->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(-100.0, -10.0,-200.0));
    node->attachObject(ent);
    node->setScale(0.1, 0.1, 0.1);

    mCamera->setPosition(90.0, 280.0, 535.0);
    mCamera->pitch(Ogre::Degree(-30.0));
    mCamera->yaw(Ogre::Degree(-15.0));

    mAnimationState = mEntity->getAnimationState("Idle");
    mAnimationState->setLoop(true);
    mAnimationState->setEnabled(true);
}

void OgreApplication::windowResized(Ogre::RenderWindow* rw) {
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);
    _inputSystemManager.setMouseArea(width, height);
}

void OgreApplication::windowClosed(Ogre::RenderWindow* rw) {
    if(rw == mWindow) {
        _inputSystemManager.destroy();
    }
}

bool OgreApplication::frameRenderingQueued(const Ogre::FrameEvent& evt) {
    _GUIManager.notifyFrameRenderingQueued(evt.timeSinceLastFrame);
    _inputSystemManager.notifyFrameRenderingQueued();
    if(mWindow->isClosed() || _gameState.isExitGame()) {
        return false;
    }
    if(mDirection == Ogre::Vector3::ZERO) {
        if(nextLocation()) {
            mAnimationState = mEntity->getAnimationState("Walk");
            mAnimationState->setLoop(true);
            mAnimationState->setEnabled(true);
        }
    } else {
        Ogre::Real move = mWalkSpd * evt.timeSinceLastFrame;
        mDistance -= move;
        if(mDistance <= 0.0) {
            mNode->setPosition(mDestination);
            mDirection = Ogre::Vector3::ZERO;
            if(nextLocation()) {
                Ogre::Vector3 src = mNode->getOrientation() * Ogre::Vector3::UNIT_X;
                if ((1.0 + src.dotProduct(mDirection)) < 0.0001) {
                    mNode->yaw(Ogre::Degree(180));
                } else {
                    Ogre::Quaternion quat = src.getRotationTo(mDirection);
                    mNode->rotate(quat);
                }
            } else {
                mAnimationState = mEntity->getAnimationState("Idle");
                mAnimationState->setLoop(true);
                mAnimationState->setEnabled(true);
            }
        } else {
            mNode->translate(move * mDirection);
        }
    }
    mAnimationState->addTime(evt.timeSinceLastFrame);
    return true;
}

void OgreApplication::ogreLog(const Ogre::String &msg) {
    Ogre::LogManager::getSingletonPtr()->logMessage(msg);
}

bool OgreApplication::nextLocation() {
    if (mWalkList.empty()) {
        return false;
    }
    mDestination = mWalkList.front();
    mWalkList.pop_front();
    mDirection = mDestination - mNode->getPosition();
    mDistance = mDirection.normalise();
    return true;
}

//----------------------------------------------------------------------------

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        OgreApplication app;

        try {
            app.go();
        } catch(Ogre::Exception& e)  {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox(NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occurred: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif

//---------------------------------------------------------------------------
