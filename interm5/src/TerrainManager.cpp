#include "TerrainManager.h"

#include <CEGUI/CEGUI.h>

TerrainManager::~TerrainManager() {
    /* TODO: adding these lines leads to exceptin on app exit. */
    // OGRE_DELETE mTerrainGroup;
    // OGRE_DELETE mTerrainGlobals;
}

void TerrainManager::setupTerrain(Ogre::SceneManager* sceneMgr, Ogre::Light* light) {
    mTerrainGlobals = OGRE_NEW Ogre::TerrainGlobalOptions();

    mTerrainGroup = OGRE_NEW Ogre::TerrainGroup(
        sceneMgr, 
        Ogre::Terrain::ALIGN_X_Z, 
        513, 12000.0
    );
    mTerrainGroup->setFilenameConvention(Ogre::String("terrain"), Ogre::String("dat"));
    mTerrainGroup->setOrigin(Ogre::Vector3::ZERO);

    configureTerrainDefaults(sceneMgr, light);

    for (long x = 0; x <= 0; ++x) {
        for (long y = 0; y <= 0; ++y) {
            defineTerrain(x, y);
        }
    }
    mTerrainGroup->loadAllTerrains(true);

    if (mTerrainsImported) {
        Ogre::TerrainGroup::TerrainIterator ti = mTerrainGroup->getTerrainIterator();
        while (ti.hasMoreElements()) {
            Ogre::Terrain* t = ti.getNext()->instance;
            initBlendMaps(t);
        }
    }

    mTerrainGroup->freeTemporaryResources();
}

float TerrainManager::getHeight(const Ogre::Vector3 &pos) {
    Ogre::Terrain **t = nullptr;
    return mTerrainGroup->getHeightAtWorldPosition(pos, t);
}

bool TerrainManager::frameRenderingQueued(const Ogre::FrameEvent& evt) {
    CEGUI::Window* terrainLabel = CEGUI::System::getSingleton()
        .getDefaultGUIContext().getRootWindow()->getChild("TerrainLabel");
 
    if(mTerrainGroup->isDerivedDataUpdateInProgress()) {
        if(mTerrainsImported) {
            terrainLabel->setText("Building terrain, please wait...");
        } else {
            terrainLabel->setText("Updating textures, patience...");
        }
    } else {
        terrainLabel->setText("Intermediate tutorial");
        if(mTerrainsImported) {
            mTerrainGroup->saveAllTerrains(true);
            mTerrainsImported = false;
        }
    }
}

void TerrainManager::handleCameraCollision(Ogre::SceneNode *cameraNode) {
    Ogre::Vector3 camPos = cameraNode->getPosition();
    Ogre::Ray camRay(Ogre::Vector3(camPos.x, 5000.0, camPos.z), Ogre::Vector3::NEGATIVE_UNIT_Y);
    Ogre::TerrainGroup::RayResult result = mTerrainGroup->rayIntersects(camRay);
    if (result.terrain) {
        Ogre::Real terrainHeight = result.position.y;
        if (camPos.y < (terrainHeight + 10.0)) {
            cameraNode->setPosition(camPos.x, terrainHeight + 10.0, camPos.z);
        }
    }
}

void TerrainManager::configureTerrainDefaults(Ogre::SceneManager *sceneMgr, Ogre::Light *light) {
    mTerrainGlobals->setMaxPixelError(8);
    mTerrainGlobals->setCompositeMapDistance(3000);
    mTerrainGlobals->setLightMapDirection(light->getDerivedDirection());
    mTerrainGlobals->setCompositeMapAmbient(sceneMgr->getAmbientLight());
    mTerrainGlobals->setCompositeMapDiffuse(light->getDiffuseColour());

    Ogre::Terrain::ImportData& importData = mTerrainGroup->getDefaultImportSettings();
    importData.terrainSize = 513;
    importData.worldSize = 12000.0;
    importData.inputScale = 600;
    importData.minBatchSize = 33;
    importData.maxBatchSize = 65;

    importData.layerList.resize(3);
    importData.layerList[0].worldSize = 100;
    importData.layerList[0].textureNames.push_back("dirt_grayrocky_diffusespecular.dds");
    importData.layerList[0].textureNames.push_back("dirt_grayrocky_normalheight.dds");
    importData.layerList[1].worldSize = 30;
    importData.layerList[1].textureNames.push_back("grass_green-01_diffusespecular.dds");
    importData.layerList[1].textureNames.push_back("grass_green-01_normalheight.dds");
    importData.layerList[2].worldSize = 200;
    importData.layerList[2].textureNames.push_back("growth_weirdfungus-03_diffusespecular.dds");
    importData.layerList[2].textureNames.push_back("growth_weirdfungus-03_normalheight.dds");
}

void getTerrainImage(bool flipX, bool flipY, Ogre::Image& img) {
    img.load("terrain.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    if (flipX) {
      img.flipAroundY();
    }
    if (flipY) {
      img.flipAroundX();
    }
}

void TerrainManager::defineTerrain(long x, long y) {
    Ogre::String filename = mTerrainGroup->generateFilename(x, y);
    bool exists = Ogre::ResourceGroupManager::getSingleton().resourceExists(
        mTerrainGroup->getResourceGroup(),
        filename
    );
    if (exists) {
        mTerrainGroup->defineTerrain(x, y);
    } else {
        Ogre::Image img;
        getTerrainImage(x % 2 != 0, y % 2 != 0, img);
        mTerrainGroup->defineTerrain(x, y, &img);
        mTerrainsImported = true;
    }
}

void TerrainManager::initBlendMaps(Ogre::Terrain* terrain) {
    Ogre::Real minHeight0 = 70;
    Ogre::Real fadeDist0 = 40;
    Ogre::Real minHeight1 = 70;
    Ogre::Real fadeDist1 = 15;
     
    Ogre::TerrainLayerBlendMap* blendMap0 = terrain->getLayerBlendMap(1);
    Ogre::TerrainLayerBlendMap* blendMap1 = terrain->getLayerBlendMap(2);
     
    float* pBlend0 = blendMap0->getBlendPointer();
    float* pBlend1 = blendMap1->getBlendPointer();
     
    for (Ogre::uint16 y = 0; y < terrain->getLayerBlendMapSize(); ++y) {
        for (Ogre::uint16 x = 0; x < terrain->getLayerBlendMapSize(); ++x) {
            Ogre::Real tx, ty;

            blendMap0->convertImageToTerrainSpace(x, y, &tx, &ty);
            Ogre::Real height = terrain->getHeightAtTerrainPosition(tx, ty);
            Ogre::Real val = (height - minHeight0) / fadeDist0;
            val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
            *pBlend0++ = val;

            val = (height - minHeight1) / fadeDist1;
            val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
            *pBlend1++ = val;
        }
    }

    blendMap0->dirty();
    blendMap1->dirty();
    blendMap0->update();
    blendMap1->update();
}