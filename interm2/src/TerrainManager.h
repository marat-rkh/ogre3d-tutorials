#ifndef __TerrainManager_h_
#define __TerrainManager_h_

#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>

class TerrainManager {
public:
    virtual ~TerrainManager();

    void setupTerrain(Ogre::SceneManager* sceneMgr, Ogre::Light* light);
    void nextFrame();
private:
    bool mTerrainsImported = false;
    Ogre::TerrainGroup* mTerrainGroup = nullptr;
    Ogre::TerrainGlobalOptions* mTerrainGlobals = nullptr;

    void defineTerrain(long x, long y);
    void initBlendMaps(Ogre::Terrain* terrain);
    void configureTerrainDefaults(Ogre::SceneManager* sceneMgr, Ogre::Light* light);
};

#endif