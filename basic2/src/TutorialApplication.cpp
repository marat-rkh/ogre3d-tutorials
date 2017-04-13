/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.cpp
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

#include "TutorialApplication.h"

//---------------------------------------------------------------------------
TutorialApplication::TutorialApplication(void)
{
}
//---------------------------------------------------------------------------
TutorialApplication::~TutorialApplication(void)
{
}

void TutorialApplication::createCamera() {
    mCamera = mSceneMgr->createCamera("PlayerCam");
    mCamera->setPosition(Ogre::Vector3(0, 300, 500));
    mCamera->lookAt(Ogre::Vector3(0, 0, 0));
    mCamera->setNearClipDistance(5);
    mCameraMan = new OgreBites::SdkCameraMan(mCamera);
}

void TutorialApplication::createViewports() {
    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));
    mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}

//---------------------------------------------------------------------------
void TutorialApplication::createScene(void) {
    // general light and shadow
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0, 0, 0));
    mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

    // create ninja
    Ogre::Entity* ninjaEntity = mSceneMgr->createEntity("ninja.mesh");
    ninjaEntity->setCastShadows(true);
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(ninjaEntity);

    // create textured ground as plane
    Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
    Ogre::MeshManager::getSingleton().createPlane(
        "ground",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        plane, 
        1500, 1500, 20, 20, 
        true, 
        1, 5, 5, 
        Ogre::Vector3::UNIT_Z
    );

    Ogre::Entity* groundEntity = mSceneMgr->createEntity("ground");
    groundEntity->setCastShadows(false);
    groundEntity->setMaterialName("Examples/Rockwall");
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(groundEntity);

    // create spot light
    Ogre::Light* spotLight = mSceneMgr->createLight("SpotLight");
    spotLight->setType(Ogre::Light::LT_SPOTLIGHT);
    spotLight->setDiffuseColour(0, 0, 1.0);
    spotLight->setSpecularColour(0, 0, 1.0);
    spotLight->setDirection(-1, -1, 0);
    spotLight->setPosition(Ogre::Vector3(200, 200, 0));
    spotLight->setSpotlightRange(Ogre::Degree(35), Ogre::Degree(60));
    setLightSourceVizualizer(Ogre::Vector3(200, 200, 0));

    // create directional light
    Ogre::Light* directionalLight = mSceneMgr->createLight("DirectionalLight");
    directionalLight->setType(Ogre::Light::LT_DIRECTIONAL);
    directionalLight->setDiffuseColour(Ogre::ColourValue(.4, 0, 0));
    directionalLight->setSpecularColour(Ogre::ColourValue(.4, 0, 0));
    directionalLight->setDirection(Ogre::Vector3(0, -1, 0));

    // create point light
    Ogre::Light* pointLight = mSceneMgr->createLight("PointLight");
    pointLight->setType(Ogre::Light::LT_POINT);
    pointLight->setDiffuseColour(.3, .3, .3);
    pointLight->setSpecularColour(.3, .3, .3);
    pointLight->setPosition(Ogre::Vector3(0, 150, 250));
    setLightSourceVizualizer(Ogre::Vector3(0, 150, 250));
}
//---------------------------------------------------------------------------


void TutorialApplication::setLightSourceVizualizer(const Ogre::Vector3 &pos) {
    // create ogre head as light source position marker
    Ogre::Entity* spotLightEntity = mSceneMgr->createEntity("ogrehead.mesh");
    spotLightEntity->setCastShadows(false);
    Ogre::SceneNode *spotLightNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    spotLightNode->setPosition(pos);
    spotLightNode->setScale(0.5, 0.5, 0.5);
    spotLightNode->attachObject(spotLightEntity);
}

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
        TutorialApplication app;

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
