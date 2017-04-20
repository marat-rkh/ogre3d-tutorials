#ifndef __RobotsCreator_h_
#define __RobotsCreator_h_

#include "InputEventListener.h"

#include <OgreCamera.h>
#include <CEGUI/MouseCursor.h>
#include <Terrain/OgreTerrainGroup.h>
#include <OgreSceneManager.h>

class RobotsCreator : public InputEventListener {
public:
    RobotsCreator(
        Ogre::Camera *camera,
        CEGUI::MouseCursor *mouseCursor,
        Ogre::TerrainGroup *terrainGroup,
        Ogre::SceneManager *sceneManager
    );

    bool mousePressed(const OIS::MouseEvent &me, OIS::MouseButtonID id) override;
    bool mouseReleased(const OIS::MouseEvent &me, OIS::MouseButtonID id) override;
    bool mouseMoved(const OIS::MouseEvent &me) override;
private:
    Ogre::Camera *_camera;
    CEGUI::MouseCursor *_mouseCursor;
    Ogre::TerrainGroup *_terrainGroup;
    Ogre::SceneManager *_sceneManager;

    Ogre::SceneNode* _currentRobotNode = nullptr;

    Ogre::TerrainGroup::RayResult castRayForCurrentMousePos(const OIS::MouseEvent &me);
};

#endif