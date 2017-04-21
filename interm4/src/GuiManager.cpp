#include "GuiManager.h"

GuiManager::GuiManager(GameState &gameState) :
    _gameState(gameState)
{}

void GuiManager::initGUISystem() {
    _renderer = &CEGUI::OgreRenderer::bootstrapSystem();
    _guiContext = &CEGUI::System::getSingleton().getDefaultGUIContext();

    CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
    CEGUI::Font::setDefaultResourceGroup("Fonts");
    CEGUI::Scheme::setDefaultResourceGroup("Schemes");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
    CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
}

void GuiManager::setupGUI() {
    CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
    CEGUI::FontManager::getSingleton().createFromFile("DejaVuSans-10.font");
    _guiContext->setDefaultFont("DejaVuSans-10");
    _guiContext->getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");

    CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::Window* rootWin = wmgr.loadLayoutFromFile("test.layout");
    _guiContext->setRootWindow(rootWin);
}

bool GuiManager::frameRenderingQueued(const Ogre::FrameEvent& evt) {
    //Need to inject timestamps to CEGUI System.
    CEGUI::System::getSingleton().injectTimePulse(evt.timeSinceLastFrame);
    return true;
}

bool GuiManager::keyPressed(const OIS::KeyEvent &arg) {
    _guiContext->injectKeyDown(static_cast<CEGUI::Key::Scan>(arg.key));
    _guiContext->injectChar(static_cast<CEGUI::Key::Scan>(arg.text));
    return true;
}

bool GuiManager::keyReleased(const OIS::KeyEvent &arg) {
    _guiContext->injectKeyUp(static_cast<CEGUI::Key::Scan>(arg.key));
    return true;
}

bool GuiManager::mouseMoved(const OIS::MouseEvent &arg) {
    _guiContext->injectMouseMove(arg.state.X.rel, arg.state.Y.rel);
    // Scroll wheel.
    if (arg.state.Z.rel) {
        _guiContext->injectMouseWheelChange(arg.state.Z.rel / 120.0f);
    }
    return true;
}

bool GuiManager::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
    _guiContext->injectMouseButtonDown(OIStoCEGUIMouseButton(id));

    _guiContext->getMouseCursor().hide();
    _mousePosition = _guiContext->getMouseCursor().getPosition();
    return true;
}

bool GuiManager::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
    _guiContext->injectMouseButtonUp(OIStoCEGUIMouseButton(id));

    _guiContext->getMouseCursor().setPosition(_mousePosition);
    _guiContext->getMouseCursor().show();
    return true;
}

CEGUI::MouseButton GuiManager::OIStoCEGUIMouseButton(const OIS::MouseButtonID &buttonID) {
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