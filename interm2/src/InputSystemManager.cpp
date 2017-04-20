#include "InputSystemManager.h"

InputSystemManager::InputSystemManager(
    GameState &gameState, 
    GuiManager &guiManager,
    Ogre::SceneNode *cameraNode
) :
    _gameState(gameState),
    _GUIManager(guiManager),
    _cameraNode(cameraNode),
    _cameraMovementDirection(Ogre::Vector3::ZERO)
{}

void InputSystemManager::init(size_t windowHnd) {
    std::ostringstream windowHndStr;
    windowHndStr << windowHnd;
    OIS::ParamList pl;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
    mInputManager = OIS::InputManager::createInputSystem(pl);
    mKeyboard = static_cast<OIS::Keyboard*>(
        mInputManager->createInputObject(OIS::OISKeyboard, /*buffered*/true)
    );
    mMouse = static_cast<OIS::Mouse*>(
        mInputManager->createInputObject(OIS::OISMouse, /*buffered*/true)
    );
    mMouse->setEventCallback(this);
    mKeyboard->setEventCallback(this);
}

void InputSystemManager::setMouseArea(unsigned int width, unsigned int height) {
    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}

void InputSystemManager::destroy() {
    if(mInputManager) {
        mInputManager->destroyInputObject(mMouse);
        mInputManager->destroyInputObject(mKeyboard);
        OIS::InputManager::destroyInputSystem(mInputManager);
        mInputManager = nullptr;
    }
}

bool InputSystemManager::frameRenderingQueued(const Ogre::FrameEvent& evt) {
    mKeyboard->capture();
    mMouse->capture();

    _cameraNode->translate(_cameraMovementDirection * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
}

bool InputSystemManager::keyPressed(const OIS::KeyEvent &arg) {
    _GUIManager.notifyKeyPressed(
        static_cast<CEGUI::Key::Scan>(arg.key),
        static_cast<CEGUI::Key::Scan>(arg.text)
    );
    switch (arg.key) {
    case OIS::KC_ESCAPE: 
        _gameState.isExitGame(true);
        break;
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

bool InputSystemManager::keyReleased(const OIS::KeyEvent &arg) {
    _GUIManager.notifyKeyReleased(static_cast<CEGUI::Key::Scan>(arg.key));
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

bool InputSystemManager::mouseMoved(const OIS::MouseEvent &arg) {
    _GUIManager.notifyMouseMoved(arg.state.X.rel, arg.state.Y.rel, arg.state.Z.rel);
    if (arg.state.buttonDown(OIS::MB_Right)) {
        _cameraNode->yaw(Ogre::Degree(-CAMERA_ROT * arg.state.X.rel), Ogre::Node::TS_WORLD);
        _cameraNode->pitch(Ogre::Degree(-CAMERA_ROT * arg.state.Y.rel), Ogre::Node::TS_LOCAL);
    }
    return true;
}

bool InputSystemManager::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
    _GUIManager.notifyMousePressed(OIStoCEGUIMouseButton(id));
    return true;
}

bool InputSystemManager::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
    _GUIManager.notifyMouseReleased(OIStoCEGUIMouseButton(id));
    return true;
}

CEGUI::MouseButton InputSystemManager::OIStoCEGUIMouseButton(const OIS::MouseButtonID &buttonID) {
    switch (buttonID) {
    case OIS::MB_Left:
        return CEGUI::LeftButton;
    case OIS::MB_Right:
        return CEGUI::RightButton;
    case OIS::MB_Middle:
        return CEGUI::MiddleButton;
    default:
        return CEGUI::LeftButton;
    }
}