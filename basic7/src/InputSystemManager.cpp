#include "InputSystemManager.h"

InputSystemManager::InputSystemManager(GameState &gameState, GuiManager &guiManager) :
    _gameState(gameState),
    _GUIManager(guiManager)
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

void InputSystemManager::notifyFrameRenderingQueued() {
    mKeyboard->capture();
    mMouse->capture();
}

bool InputSystemManager::keyPressed(const OIS::KeyEvent &arg) {
    _GUIManager.notifyKeyPressed(
        static_cast<CEGUI::Key::Scan>(arg.key),
        static_cast<CEGUI::Key::Scan>(arg.text)
    );
    if(arg.key == OIS::KC_ESCAPE) { 
        _gameState.isExitGame(true);
    }
    return true;
}

bool InputSystemManager::keyReleased(const OIS::KeyEvent &arg) {
    _GUIManager.notifyKeyReleased(static_cast<CEGUI::Key::Scan>(arg.key));
    return true;
}

bool InputSystemManager::mouseMoved(const OIS::MouseEvent &arg) {
    _GUIManager.notifyMouseMoved(arg.state.X.rel, arg.state.Y.rel, arg.state.Z.rel);
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