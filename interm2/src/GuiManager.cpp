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

void GuiManager::notifyFrameRenderingQueued(Ogre::Real timeSinceLastFrame) {
    //Need to inject timestamps to CEGUI System.
    CEGUI::System::getSingleton().injectTimePulse(timeSinceLastFrame);
}

void GuiManager::notifyKeyPressed(CEGUI::Key::Scan code, CEGUI::Key::Scan text) {
    _guiContext->injectKeyDown(code);
    _guiContext->injectChar(text);
}

void GuiManager::notifyKeyReleased(CEGUI::Key::Scan code) {
    CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp(code);
}

void GuiManager::notifyMouseMoved(float deltaX, float deltaY, float deltaWheel) {
    _guiContext->injectMouseMove(deltaX, deltaY);
    // Scroll wheel.
    if (deltaWheel) {
        _guiContext->injectMouseWheelChange(deltaWheel / 120.0f);
    }
}

void GuiManager::notifyMousePressed(CEGUI::MouseButton id) {
    _guiContext->injectMouseButtonDown(id);

    _guiContext->getMouseCursor().hide();
    _mousePosition = _guiContext->getMouseCursor().getPosition();
}

void GuiManager::notifyMouseReleased(CEGUI::MouseButton id) {
    _guiContext->injectMouseButtonUp(id);

    _guiContext->getMouseCursor().setPosition(_mousePosition);
    _guiContext->getMouseCursor().show();
}

void GuiManager::pressExitButton() {
    _gameState.isExitGame(true);
}