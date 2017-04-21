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

    virtual ~RobotsCreator();

    bool mousePressed(const OIS::MouseEvent &me, OIS::MouseButtonID id) override;
    bool mouseReleased(const OIS::MouseEvent &me, OIS::MouseButtonID id) override;
    bool mouseMoved(const OIS::MouseEvent &me) override;

    bool keyPressed(const OIS::KeyEvent &arg) override;
private:
    Ogre::Camera *_camera;
    CEGUI::MouseCursor *_mouseCursor;
    Ogre::TerrainGroup *_terrainGroup;
    Ogre::SceneManager *_sceneManager;
    Ogre::RaySceneQuery* _raySceneQuery;

    bool _leftMBPressed = false;
    Ogre::SceneNode* _currentObjectNode = nullptr;
    Ogre::SceneNode* _selectedNode = nullptr;

    bool _robotMode = true;

    Ogre::Ray castRayForCurrentMousePos(const OIS::MouseEvent &me);
    Ogre::SceneNode* getSceneNodeUnderRay(Ogre::Ray ray);

    const Ogre::uint32 ROBOT_MASK = 1 << 0;
    const Ogre::uint32 NINJA_MASK = 1 << 1;
};

#endif