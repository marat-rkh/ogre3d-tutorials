#include "GuiManager.h"

GuiManager::GuiManager(GameState &gameState) :
    _gameState(gameState)
{}

void GuiManager::initGUISystem() {
    _renderer = &CEGUI::OgreRenderer::bootstrapSystem();

    CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
    CEGUI::Font::setDefaultResourceGroup("Fonts");
    CEGUI::Scheme::setDefaultResourceGroup("Schemes");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
    CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
}

void GuiManager::setupGUI() {
    CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
    CEGUI::FontManager::getSingleton().createFromFile("DejaVuSans-10.font");
    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    context.setDefaultFont("DejaVuSans-10");
    context.getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");

    CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::Window* rootWin = wmgr.loadLayoutFromFile("test.layout");
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(rootWin);
}

void GuiManager::notifyFrameRenderingQueued(Ogre::Real timeSinceLastFrame) {
    //Need to inject timestamps to CEGUI System.
    CEGUI::System::getSingleton().injectTimePulse(timeSinceLastFrame);
}

void GuiManager::notifyKeyPressed(CEGUI::Key::Scan code, CEGUI::Key::Scan text) {
    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    context.injectKeyDown(code);
    context.injectChar(text);
}

void GuiManager::notifyKeyReleased(CEGUI::Key::Scan code) {
    CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp(code);
}

void GuiManager::notifyMouseMoved(float deltaX, float deltaY, float deltaWheel) {
    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    context.injectMouseMove(deltaX, deltaY);
    // Scroll wheel.
    if (deltaWheel) {
        context.injectMouseWheelChange(deltaWheel / 120.0f);
    }
}

void GuiManager::notifyMousePressed(CEGUI::MouseButton id) {
    CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(id);
}

void GuiManager::notifyMouseReleased(CEGUI::MouseButton id) {
    CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(id);
}

void GuiManager::pressExitButton() {
    _gameState.isExitGame(true);
}