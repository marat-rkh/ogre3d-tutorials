#ifndef __BasicCameraManager_h_
#define __BasicCameraManager_h_

#include "InputEventListener.h"

#include <OgreVector3.h>
#include <OgreSceneNode.h>
#include <OgreFrameListener.h>

class BasicCameraManager : public InputEventListener {
public:
    BasicCameraManager(Ogre::SceneNode *cameraNode);

    bool frameRenderingQueued(const Ogre::FrameEvent& evt);

    bool keyPressed(const OIS::KeyEvent &arg) override;
    bool keyReleased(const OIS::KeyEvent &arg) override;
    bool mouseMoved(const OIS::MouseEvent &arg) override;
private:
    Ogre::SceneNode *_cameraNode;
    Ogre::Vector3 _cameraMovementDirection;
    const Ogre::Real CAMERA_ROT = 0.13;
    const Ogre::Real CAMERA_MOV_SPEED = 250;
};

#endif // #ifndef __BasicCameraManager_h_