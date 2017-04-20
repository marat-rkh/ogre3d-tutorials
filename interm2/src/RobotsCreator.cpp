#include "RobotsCreator.h"

#include <OgreEntity.h>

RobotsCreator::RobotsCreator(
    Ogre::Camera *camera,
    CEGUI::MouseCursor *mouseCursor,
    Ogre::TerrainGroup *terrainGroup,
    Ogre::SceneManager *sceneManager
) :
    _camera(camera),
    _mouseCursor(mouseCursor),
    _terrainGroup(terrainGroup),
    _sceneManager(sceneManager)
{}

bool RobotsCreator::mousePressed(const OIS::MouseEvent &me, OIS::MouseButtonID id) {
    if(id == OIS::MB_Left) {
        Ogre::TerrainGroup::RayResult result = castRayForCurrentMousePos(me);
        if (result.terrain) {
            _currentRobotNode = _sceneManager->getRootSceneNode()->createChildSceneNode();
            _currentRobotNode->setPosition(result.position);
            _currentRobotNode->setScale(0.2, 0.2, 0.2);
            _currentRobotNode->attachObject(
                _sceneManager->createEntity("robot.mesh")
            );
        }
    }
    return true; 
}

bool RobotsCreator::mouseReleased(const OIS::MouseEvent &me, OIS::MouseButtonID id) {
    if(id == OIS::MB_Left) {
        _currentRobotNode = nullptr;
    }
    return true; 
}

bool RobotsCreator::mouseMoved(const OIS::MouseEvent &me) {
    if(_currentRobotNode) {
        Ogre::TerrainGroup::RayResult result = castRayForCurrentMousePos(me);
        if(result.terrain) {
            _currentRobotNode->setPosition(result.position);
        }
    }
}

Ogre::TerrainGroup::RayResult RobotsCreator::castRayForCurrentMousePos(const OIS::MouseEvent &me) {
    CEGUI::Vector2f mousePos = _mouseCursor->getPosition();
    Ogre::Ray mouseRay = _camera->getCameraToViewportRay(
        mousePos.d_x / float(me.state.width),
        mousePos.d_y / float(me.state.height)
    );
    return _terrainGroup->rayIntersects(mouseRay);
}