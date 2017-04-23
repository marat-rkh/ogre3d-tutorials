#include "BasicCameraManager.h"

BasicCameraManager::BasicCameraManager(Ogre::SceneNode *cameraNode) :
    _cameraNode(cameraNode),
    _cameraMovementDirection(Ogre::Vector3::ZERO)
{}

bool BasicCameraManager::frameRenderingQueued(const Ogre::FrameEvent& evt) {
    _cameraNode->translate(_cameraMovementDirection * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
}

bool BasicCameraManager::keyPressed(const OIS::KeyEvent &arg) {
    switch (arg.key) {
    case OIS::KC_UP:
    case OIS::KC_W:
        _cameraMovementDirection.z = -CAMERA_MOV_SPEED;
        break;
    case OIS::KC_DOWN:
    case OIS::KC_S:
        _cameraMovementDirection.z = CAMERA_MOV_SPEED;
        break;
    case OIS::KC_LEFT:
    case OIS::KC_A:
        _cameraMovementDirection.x = -CAMERA_MOV_SPEED;
        break;
    case OIS::KC_RIGHT:
    case OIS::KC_D:
        _cameraMovementDirection.x = CAMERA_MOV_SPEED;
        break;
    case OIS::KC_PGDOWN:
    case OIS::KC_E:
        _cameraMovementDirection.y = -CAMERA_MOV_SPEED;
        break;
    case OIS::KC_PGUP:
    case OIS::KC_Q:
        _cameraMovementDirection.y = CAMERA_MOV_SPEED;
        break;
    default:
        break;
    }
    return true;
}

bool BasicCameraManager::keyReleased(const OIS::KeyEvent &arg) {
    switch (arg.key) {
    case OIS::KC_UP:
    case OIS::KC_W:
        if (_cameraMovementDirection.z < 0) {
            _cameraMovementDirection.z = 0;
        }
        break;
    case OIS::KC_DOWN:
    case OIS::KC_S:
        if (_cameraMovementDirection.z > 0) {
            _cameraMovementDirection.z = 0;
        }
        break;
    case OIS::KC_LEFT:
    case OIS::KC_A:
        if (_cameraMovementDirection.x < 0) {
            _cameraMovementDirection.x = 0;
        }
        break;
    case OIS::KC_RIGHT:
    case OIS::KC_D:
        if (_cameraMovementDirection.x > 0) {
            _cameraMovementDirection.x = 0;
        }
        break;
    case OIS::KC_PGDOWN:
    case OIS::KC_E:
        if (_cameraMovementDirection.y < 0) {
            _cameraMovementDirection.y = 0;
        }
        break;
    case OIS::KC_PGUP:
    case OIS::KC_Q:
        if (_cameraMovementDirection.y > 0) {
            _cameraMovementDirection.y = 0;
        }
        break;
    default:
        break;
    }
    return true;
}

bool BasicCameraManager::mouseMoved(const OIS::MouseEvent &arg) {
    if (arg.state.buttonDown(OIS::MB_Right)) {
        _cameraNode->yaw(Ogre::Degree(-CAMERA_ROT * arg.state.X.rel), Ogre::Node::TS_WORLD);
        _cameraNode->pitch(Ogre::Degree(-CAMERA_ROT * arg.state.Y.rel), Ogre::Node::TS_LOCAL);
    }
    return true;
}