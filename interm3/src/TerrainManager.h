#ifndef __TerrainManager_h_
#define __TerrainManager_h_

#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>

#include <OgreSceneNode.h>

class TerrainManager {
public:
    virtual ~TerrainManager();

    void setupTerrain(Ogre::SceneManager *sceneMgr, Ogre::Light *light);
    bool frameRenderingQueued(const Ogre::FrameEvent& evt);
    void handleCameraCollision(Ogre::SceneNode *cameraNode);
    Ogre::TerrainGroup* terrainGroup() { return mTerrainGroup; }
private:
    bool mTerrainsImported = false;
    Ogre::TerrainGroup* mTerrainGroup = nullptr;
    Ogre::TerrainGlobalOptions* mTerrainGlobals = nullptr;

    void defineTerrain(long x, long y);
    void initBlendMaps(Ogre::Terrain* terrain);
    void configureTerrainDefaults(Ogre::SceneManager* sceneMgr, Ogre::Light* light);
};

#endif