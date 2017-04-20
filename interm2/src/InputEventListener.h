#ifndef __InputEventListener_h_
#define __InputEventListener_h_

#include <OISKeyboard.h>
#include <OISMouse.h>

class InputEventListener : 
    public OIS::KeyListener,
    public OIS::MouseListener 
{
public:
    // OIS::KeyListener
    bool keyPressed(const OIS::KeyEvent &arg) override { return true; }
    bool keyReleased(const OIS::KeyEvent &arg) override { return true; }
    // OIS::MouseListener
    bool mouseMoved(const OIS::MouseEvent &arg) override { return true; }
    bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) override { return true; }
    bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) override { return true; }
};

#endif