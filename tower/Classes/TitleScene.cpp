#include "TitleScene.h"
#include "TitleLayerLoader.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

CCScene* TitleScene::scene()
{
    CCScene *scene = CCScene::create();
    TitleScene *layer = TitleScene::create();
    scene->addChild(layer);
    
    return scene;
}

bool TitleScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCNode* node = NULL;
    
    CCNodeLoaderLibrary* ccNodeLoaderLibrary = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
    ccNodeLoaderLibrary->registerCCNodeLoader("TitleLayer", TitleLayerLoader::loader());
    CCBReader* ccbReader = new CCBReader(ccNodeLoaderLibrary);
    ccbReader->autorelease();
    node = ccbReader->readNodeGraphFromFile("TitleLayer.ccbi");
    node->setPosition(ccp(0, 0));
    this->addChild(node);
   
    return true;
}