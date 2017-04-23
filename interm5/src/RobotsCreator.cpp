#include "RobotsCreator.h"

#include <OgreEntity.h>
#include <utility>

RobotsCreator::RobotsCreator(
    Ogre::Camera *camera,
    CEGUI::MouseCursor *mouseCursor,
    Ogre::TerrainGroup *terrainGroup,
    Ogre::SceneManager *sceneManager,
    GameState &gameState
) :
    _camera(camera),
    _mouseCursor(mouseCursor),
    _terrainGroup(terrainGroup),
    _sceneManager(sceneManager),
    _raySceneQuery(_sceneManager->createRayQuery(Ogre::Ray())),
    _gameState(gameState),
    _selectionBox("SelectionBox"),
    _volumeQuery(_sceneManager->createPlaneBoundedVolumeQuery(Ogre::PlaneBoundedVolumeList()))
{
    _sceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(&_selectionBox);
}

RobotsCreator::~RobotsCreator() {
    _sceneManager->destroyQuery(_raySceneQuery);
    _sceneManager->destroyQuery(_volumeQuery);
}

bool RobotsCreator::mousePressed(const OIS::MouseEvent &me, OIS::MouseButtonID id) {
    _leftMBPressed = true;
    if(id == OIS::MB_Left) {
        Ogre::Vector2 mousePos = getMousePos(me);
        deselectAllNodes(_selectedNodes);
        if(_gameState.mode() == GameState::Mode::OBJECTS_CREATION) {
            // process individual object (select or create)
            findNodesInBox(_selectedNodes, mousePos, mousePos);
            if(_selectedNodes.empty()) {
                Ogre::Ray mouseRay = _camera->getCameraToViewportRay(mousePos.x, mousePos.y);
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
                    _selectedNodes.push_back(_currentObjectNode);
                }
            }
            selectAllNodes(_selectedNodes);
        } else if (_gameState.mode() == GameState::Mode::BOX_SELECTION) {
            // mass selection
            _boxSelectionInProgress = true;
            _selectionBox.clear();
            _selectionBox.setVisible(true);
            _selectionStart = mousePos;
            _selectionStop = mousePos;
        }
    }
    return true; 
}

bool RobotsCreator::mouseReleased(const OIS::MouseEvent &me, OIS::MouseButtonID id) {
    _leftMBPressed = false;
    if(id == OIS::MB_Left) {
        if(_gameState.mode() == GameState::Mode::OBJECTS_CREATION) {
            _currentObjectNode = nullptr;
        } else if (_gameState.mode() == GameState::Mode::BOX_SELECTION) {
            findNodesInBox(_selectedNodes, _selectionStart, _selectionStop);
            selectAllNodes(_selectedNodes);
            _boxSelectionInProgress = false;
            _selectionBox.setVisible(false);
        }
    }
    return true;
}

bool RobotsCreator::mouseMoved(const OIS::MouseEvent &me) {
    if(_leftMBPressed) {
        Ogre::Vector2 mousePos = getMousePos(me);
        if(_gameState.mode() == GameState::Mode::OBJECTS_CREATION) {
            Ogre::Ray mouseRay = _camera->getCameraToViewportRay(mousePos.x, mousePos.y);
            Ogre::TerrainGroup::RayResult result = _terrainGroup->rayIntersects(mouseRay);
            if(result.terrain && _currentObjectNode) {
                _currentObjectNode->setPosition(result.position);
            }
        } else if (_gameState.mode() == GameState::Mode::BOX_SELECTION && _boxSelectionInProgress) {
            _selectionStop = mousePos;
            _selectionBox.setCorners(_selectionStart, _selectionStop);
        }
    }
}

bool RobotsCreator::keyPressed(const OIS::KeyEvent &arg) {
    if(arg.key == OIS::KC_SPACE) {
        _robotMode = !_robotMode;
    }
    return true;
}

Ogre::Vector2 RobotsCreator::getMousePos(const OIS::MouseEvent &me) {
    Ogre::Vector2 p;
    p.x = _mouseCursor->getPosition().d_x / (float)me.state.width;
    p.y = _mouseCursor->getPosition().d_y / (float)me.state.height;
    return p;
}

void RobotsCreator::findNodesInBox(std::list<Ogre::SceneNode*> &nodes, Ogre::Vector2 &start, Ogre::Vector2 &stop) {
    float left = start.x;
    float right = stop.x;
    float top = start.y;
    float bottom = stop.y;
    if (left > right) {
        std::swap(left, right);
    }
    if (top > bottom) {
        std::swap(top, bottom);
    }
    if ((right - left) * (bottom - top) < 0.0001) {
        Ogre::Ray r = _camera->getCameraToViewportRay(left, top);
        Ogre::SceneNode *node = getSceneNodeUnderRay(r);
        if(node) {
            nodes.push_back(node);
        }
    } else {
        Ogre::Ray topLeft = _camera->getCameraToViewportRay(left, top);
        Ogre::Ray topRight = _camera->getCameraToViewportRay(right, top);
        Ogre::Ray bottomLeft = _camera->getCameraToViewportRay(left, bottom);
        Ogre::Ray bottomRight = _camera->getCameraToViewportRay(right, bottom);
        Ogre::Plane frontPlane = Ogre::Plane(
            topLeft.getOrigin(),
            topRight.getOrigin(),
            bottomRight.getOrigin());
        Ogre::Plane topPlane = Ogre::Plane(
            topLeft.getOrigin(),
            topLeft.getPoint(10),
            topRight.getPoint(10));
        Ogre::Plane leftPlane = Ogre::Plane(
            topLeft.getOrigin(),
            bottomLeft.getPoint(10),
            topLeft.getPoint(10));
        Ogre::Plane bottomPlane = Ogre::Plane(
            bottomLeft.getOrigin(),
            bottomRight.getPoint(10),
            bottomLeft.getPoint(10));
        Ogre::Plane rightPlane = Ogre::Plane(
            topRight.getOrigin(),
            topRight.getPoint(10),
            bottomRight.getPoint(10));
        Ogre::PlaneBoundedVolume vol;
        vol.planes.push_back(frontPlane);
        vol.planes.push_back(topPlane);
        vol.planes.push_back(leftPlane);
        vol.planes.push_back(bottomPlane);
        vol.planes.push_back(rightPlane);
        Ogre::PlaneBoundedVolumeList volList;
        volList.push_back(vol);

        _volumeQuery->setVolumes(volList);
        Ogre::SceneQueryResult result = _volumeQuery->execute();
        for(auto m : result.movables) {
            if(m && m->getName() != "" && m->getName() != "MainCam") {
                nodes.push_back(m->getParentSceneNode());
            }
        }
    }
}

Ogre::SceneNode* RobotsCreator::getSceneNodeUnderRay(Ogre::Ray ray) {
    _raySceneQuery->setRay(ray);
    _raySceneQuery->setSortByDistance(true);
    _raySceneQuery->setQueryMask(_robotMode ? ROBOT_MASK : NINJA_MASK);
    Ogre::RaySceneQueryResult& result = _raySceneQuery->execute();
    for (auto &entry : result) {
        auto m = entry.movable;
        if(m && m->getName() != "" && m->getName() != "MainCam") {
            return m->getParentSceneNode();
        }
    }
    return nullptr;
}

void RobotsCreator::selectAllNodes(std::list<Ogre::SceneNode*> &nodes) {
    for (auto n : nodes) {
        n->showBoundingBox(true);
    }
}

void RobotsCreator::deselectAllNodes(std::list<Ogre::SceneNode*> &nodes) {
    for (auto n : nodes) {
        n->showBoundingBox(false);
    }
    nodes.clear();
}