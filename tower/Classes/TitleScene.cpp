/*
 * 担当者：北島 領人
 */

#include "TitleScene.h"
#include "TitleLayerLoader.h"
#include "cocos-ext.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace CocosDenshion;

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
    
    // BGM、効果音のロード
    SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.5);
    SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("sound/title_bgm.mp3");
    SimpleAudioEngine::sharedEngine()->preloadEffect("sound/start.wav");
    
    CCNode* node = NULL;
    
    CCNodeLoaderLibrary* ccNodeLoaderLibrary = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
    ccNodeLoaderLibrary->registerCCNodeLoader("TitleLayer", TitleLayerLoader::loader());
    CCBReader* ccbReader = new CCBReader(ccNodeLoaderLibrary);
    ccbReader->autorelease();
    node = ccbReader->readNodeGraphFromFile("TitleLayer.ccbi");
    node->setPosition(ccp(0, 0));
    this->addChild(node);
    
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic("sound/title_bgm.mp3");
   
    return true;
}