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
    _sceneManager(sceneManager),
    _raySceneQuery(_sceneManager->createRayQuery(Ogre::Ray()))
{}

RobotsCreator::~RobotsCreator() {
    _sceneManager->destroyQuery(_raySceneQuery);
}

bool RobotsCreator::mousePressed(const OIS::MouseEvent &me, OIS::MouseButtonID id) {
    _leftMBPressed = true;
    if(id == OIS::MB_Left) {
        if(_selectedNode) {
            _selectedNode->showBoundingBox(false);
        }
        Ogre::Ray mouseRay = castRayForCurrentMousePos(me);
        _selectedNode = getSceneNodeUnderRay(mouseRay);
        if(!_selectedNode) {
            Ogre::TerrainGroup::RayResult result = _terrainGroup->rayIntersects(mouseRay);
            if (result.terrain) {
                _currentObjectNode = _sceneManager->getRootSceneNode()->createChildSceneNode();
                _currentObjectNode->setPosition(result.position);
                if(_robotMode) {
                    _currentObjectNode->setScale(0.45, 0.45, 0.45);
                    Ogre::Entity *ent = _sceneManager->createEntity("robot.mesh");
                    ent->setQueryFlags(ROBOT_MASK);
                    _currentObjectNode->attachObject(ent);
                } else {
                    _currentObjectNode->setScale(0.2, 0.2, 0.2);
                    Ogre::Entity *ent = _sceneManager->createEntity("ninja.mesh");
                    ent->setQueryFlags(NINJA_MASK);
                    _currentObjectNode->attachObject(ent);
                }
                _selectedNode = _currentObjectNode;
            }
        }
        if(_selectedNode) {
            _selectedNode->showBoundingBox(true);
        }
    }
    return true; 
}

bool RobotsCreator::mouseReleased(const OIS::MouseEvent &me, OIS::MouseButtonID id) {
    _leftMBPressed = false;
    if(id == OIS::MB_Left) {
        _currentObjectNode = nullptr;
    }
    return true;
}

bool RobotsCreator::mouseMoved(const OIS::MouseEvent &me) {
    if(_leftMBPressed && _currentObjectNode) {
        Ogre::Ray mouseRay = castRayForCurrentMousePos(me);
        Ogre::TerrainGroup::RayResult result = _terrainGroup->rayIntersects(mouseRay);
        if(result.terrain) {
            _currentObjectNode->setPosition(result.position);
        }
    }
}

bool RobotsCreator::keyPressed(const OIS::KeyEvent &arg) {
    if(arg.key == OIS::KC_SPACE) {
        _robotMode = !_robotMode;
    }
    return true;
}

Ogre::Ray RobotsCreator::castRayForCurrentMousePos(const OIS::MouseEvent &me) {
    CEGUI::Vector2f mousePos = _mouseCursor->getPosition();
    return _camera->getCameraToViewportRay(
        mousePos.d_x / float(me.state.width),
        mousePos.d_y / float(me.state.height)
    );
}

Ogre::SceneNode* RobotsCreator::getSceneNodeUnderRay(Ogre::Ray ray) {
    _raySceneQuery->setRay(ray);
    _raySceneQuery->setSortByDistance(true);
    _raySceneQuery->setQueryMask(_robotMode ? ROBOT_MASK : NINJA_MASK);
    Ogre::RaySceneQueryResult& rayQueryRes = _raySceneQuery->execute();
    for (auto &entry : rayQueryRes) {
        Ogre::MovableObject *mo = entry.movable;
        if(mo && mo->getName() != "" && mo->getName() != "MainCam") {
            return mo->getParentSceneNode();
        }
    }
    return nullptr;
}