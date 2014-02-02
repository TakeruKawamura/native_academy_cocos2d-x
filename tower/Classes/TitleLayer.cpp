/*
 * 担当者：北島 領人
 */

#include "TitleLayer.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

SEL_MenuHandler TitleLayer::onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "tappedStart", TitleLayer::tappedStart);
    
    return NULL;
}

void TitleLayer::tappedStart(CCObject* pSender, CCControlEvent pCCControlEvent)
{
    SimpleAudioEngine::sharedEngine()->playEffect("sound/start.wav");
    SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(true);
    
    CCScene* game = GameScene::scene();
    CCTransitionFade* tran = CCTransitionFade::create(2.0f, game, ccc3(0, 0, 0));
    CCDirector::sharedDirector()->replaceScene(tran);
}


SEL_CCControlHandler TitleLayer::onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName)
{
    return NULL;
}
