#include "InputSystemManager.h"

InputSystemManager::InputSystemManager(
    GameState &gameState
) :
    _gameState(gameState)
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
}

bool InputSystemManager::keyPressed(const OIS::KeyEvent &arg) {
    switch (arg.key) {
    case OIS::KC_ESCAPE: 
        _gameState.isExitGame(true);
        break;
    default:
        break;
    }
    for(auto l : _listeners) {
        l->keyPressed(arg);
    }
    return true;
}

bool InputSystemManager::keyReleased(const OIS::KeyEvent &arg) {
    for(auto l : _listeners) {
        l->keyReleased(arg);
    }
    return true;
}

bool InputSystemManager::mouseMoved(const OIS::MouseEvent &arg) {
    for(auto l : _listeners) {
        l->mouseMoved(arg);
    }
    return true;
}

bool InputSystemManager::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
    for(auto l : _listeners) {
        l->mousePressed(arg, id);
    }
    return true;
}

bool InputSystemManager::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
    for(auto l : _listeners) {
        l->mouseReleased(arg, id);
    }
    return true;
}