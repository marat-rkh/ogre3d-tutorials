/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.h
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/
Tutorial Framework (for Ogre 1.9)
http://www.ogre3d.org/wiki/
-----------------------------------------------------------------------------
*/

#ifndef __TutorialApplication_h_
#define __TutorialApplication_h_

#include "BaseApplication.h"

//---------------------------------------------------------------------------

class TutorialApplication : public BaseApplication
{
public:
    TutorialApplication(void);
    virtual ~TutorialApplication(void);

private:
    void createScene(void) override;
    bool frameRenderingQueued(const Ogre::FrameEvent& evt) override;

    bool keyPressed(const OIS::KeyEvent& ke) override;
    bool keyReleased(const OIS::KeyEvent& ke) override;

    bool mouseMoved(const OIS::MouseEvent& me) override;
    bool mousePressed(const OIS::MouseEvent& me, OIS::MouseButtonID id) override;
    bool mouseReleased(const OIS::MouseEvent& me, OIS::MouseButtonID id) override;

    Ogre::Real mRotate;
    Ogre::Real mMove;
    Ogre::SceneNode* mCamNode;
    Ogre::Vector3 mDirection;
};

//---------------------------------------------------------------------------

#endif // #ifndef __TutorialApplication_h_

//---------------------------------------------------------------------------
