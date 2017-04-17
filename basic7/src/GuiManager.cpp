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
    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor()
        .setDefaultImage("TaharezLook/MouseArrow");

    CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::Window *sheet = wmgr.createWindow("DefaultWindow", "CEGUIDemo/Sheet");
    CEGUI::Window *quit = wmgr.createWindow("TaharezLook/Button", "CEGUIDemo/QuitButton");
    quit->setText("Quit");
    quit->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
    quit->subscribeEvent(
        CEGUI::PushButton::EventClicked, 
        CEGUI::Event::Subscriber([this](){ this->pressExitButton(); })
    );
    sheet->addChild(quit);
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);
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