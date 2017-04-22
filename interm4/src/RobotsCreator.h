#ifndef __RobotsCreator_h_
#define __RobotsCreator_h_

#include "InputEventListener.h"
#include "GameState.h"
#include "SelectionBox.h"

#include <OgreCamera.h>
#include <CEGUI/MouseCursor.h>
#include <Terrain/OgreTerrainGroup.h>
#include <OgreSceneManager.h>

#include <vector>
#include <list>

// This class is something like level editor
class RobotsCreator : public InputEventListener {
public:
    RobotsCreator(
        Ogre::Camera *camera,
        CEGUI::MouseCursor *mouseCursor,
        Ogre::TerrainGroup *terrainGroup,
        Ogre::SceneManager *sceneManager,
        GameState &gameState
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

    GameState &_gameState;

    bool _leftMBPressed = false;
    Ogre::SceneNode* _currentObjectNode = nullptr;
    std::list<Ogre::SceneNode*> _selectedNodes;

    bool _robotMode = true;

    SelectionBox _selectionBox;
    Ogre::PlaneBoundedVolumeListSceneQuery *_volumeQuery;
    bool _boxSelectionInProgress = false;
    Ogre::Vector2 _selectionStart;
    Ogre::Vector2 _selectionStop;

    const Ogre::uint32 ROBOT_MASK = 1 << 0;
    const Ogre::uint32 NINJA_MASK = 1 << 1;

    Ogre::Vector2 getMousePos(const OIS::MouseEvent &me);
    void findNodesInBox(std::list<Ogre::SceneNode*> &nodes, Ogre::Vector2 &start, Ogre::Vector2 &stop);
    Ogre::SceneNode* getSceneNodeUnderRay(Ogre::Ray ray);
    void selectAllNodes(std::list<Ogre::SceneNode*> &nodes);
    void deselectAllNodes(std::list<Ogre::SceneNode*> &nodes);
};

#endif