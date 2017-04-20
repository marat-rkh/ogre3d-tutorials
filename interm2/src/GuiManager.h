#ifndef __GuiManager_h_
#define __GuiManager_h_

#include <atomic>

#include <OgrePrerequisites.h>
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>

#include "GameState.h"

class GuiManager {
public:
    GuiManager(GameState &gameState);
    
    void initGUISystem();
    void setupGUI();
    void notifyFrameRenderingQueued(Ogre::Real timeSinceLastFrame);
    void notifyKeyPressed(CEGUI::Key::Scan code, CEGUI::Key::Scan text);
    void notifyKeyReleased(CEGUI::Key::Scan code);
    void notifyMouseMoved(float deltaX, float deltaY, float deltaWheel);
    void notifyMousePressed(CEGUI::MouseButton id);
    void notifyMouseReleased(CEGUI::MouseButton id);
    void pressExitButton();
private:
    GameState &_gameState;
    CEGUI::OgreRenderer* _renderer = nullptr;

    CEGUI::GUIContext *_guiContext = nullptr;
    CEGUI::Vector2f _mousePosition;
};

#endif // #ifndef __GuiManager_h_