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
        if(_prevRobotNode) {
            _prevRobotNode->showBoundingBox(false);
        }
        Ogre::TerrainGroup::RayResult result = castRayForCurrentMousePos(me);
        if (result.terrain) {
            _currentRobotNode = _sceneManager->getRootSceneNode()->createChildSceneNode();
            _currentRobotNode->setPosition(result.position);
            if(_robotMode) {
                _currentRobotNode->setScale(0.45, 0.45, 0.45);
                _currentRobotNode->attachObject(
                    _sceneManager->createEntity("robot.mesh")
                );
            } else {
                _currentRobotNode->setScale(0.2, 0.2, 0.2);
                _currentRobotNode->attachObject(
                    _sceneManager->createEntity("ninja.mesh")
                );
            }
            _currentRobotNode->showBoundingBox(true);
        }
    }
    return true; 
}

bool RobotsCreator::mouseReleased(const OIS::MouseEvent &me, OIS::MouseButtonID id) {
    if(id == OIS::MB_Left) {
        _prevRobotNode = _currentRobotNode;
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

bool RobotsCreator::keyPressed(const OIS::KeyEvent &arg) {
    if(arg.key == OIS::KC_SPACE) {
        _robotMode = !_robotMode;
    }
    return true;
}

Ogre::TerrainGroup::RayResult RobotsCreator::castRayForCurrentMousePos(const OIS::MouseEvent &me) {
    CEGUI::Vector2f mousePos = _mouseCursor->getPosition();
    Ogre::Ray mouseRay = _camera->getCameraToViewportRay(
        mousePos.d_x / float(me.state.width),
        mousePos.d_y / float(me.state.height)
    );
    return _terrainGroup->rayIntersects(mouseRay);
}