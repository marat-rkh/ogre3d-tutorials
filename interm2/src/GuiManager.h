#ifndef __GuiManager_h_
#define __GuiManager_h_

#include <atomic>

#include <OgrePrerequisites.h>
#include <OgreFrameListener.h>
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>
#include <CEGUI/MouseCursor.h>

#include "GameState.h"
#include "InputEventListener.h"

class GuiManager : public InputEventListener {
public:
    GuiManager(GameState &gameState);
    
    void initGUISystem();
    void setupGUI();
    bool frameRenderingQueued(const Ogre::FrameEvent& evt);

    bool keyPressed(const OIS::KeyEvent &arg) override;
    bool keyReleased(const OIS::KeyEvent &arg) override;
    bool mouseMoved(const OIS::MouseEvent &arg) override;
    bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) override;
    bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) override;

    CEGUI::MouseCursor& mouseCursor() { return _guiContext->getMouseCursor(); }
private:
    GameState &_gameState;
    CEGUI::OgreRenderer* _renderer = nullptr;

    CEGUI::GUIContext *_guiContext = nullptr;
    CEGUI::Vector2f _mousePosition;

    CEGUI::MouseButton OIStoCEGUIMouseButton(const OIS::MouseButtonID &buttonID);
};

#endif // #ifndef __GuiManager_h_