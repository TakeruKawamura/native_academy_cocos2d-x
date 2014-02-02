//
//  GameResult.cpp
//  tower
//
//  Created by cocos2d-x on 14/01/26.
//
//  Author Takeru Kawamura
//

#include "GameResult.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

CCScene* GameResult::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    GameResult *layer = GameResult::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool GameResult::init()
{
    CCLOG("call init");
    
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    CCSprite* pSprite1 = CCSprite::create("grave.png");
    
    pSprite1->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    
    this->addChild(pSprite1, 0);
    this->addGameover();
    this->scheduleOnce(schedule_selector(GameResult::showScore), 2.0f);
    
    // BGM、効果音のロード
    SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.5);
    SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("sound/gameover_bgm.mp3");
    SimpleAudioEngine::sharedEngine()->preloadEffect("sound/gameover.mp3");
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic("sound/gameover_bgm.mp3");

    return true;
}

void GameResult::addGameover()
{
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    CCLabelTTF* pLabel = CCLabelTTF::create("GAME OVER", "Arial", 50);
    pLabel->setColor(ccc3(200, 0, 0));
    pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                            visibleSize.height*1.3));
    pLabel->setTag(2);
    this->addChild(pLabel, 1);

    float duration = 1.0f;
    CCMoveTo* actionMove1 = CCMoveTo::create(duration, ccp(
                                                          pLabel->getPositionX(),
                                                          origin.y + visibleSize.height*1/2));
    duration = 0.5f;
    CCMoveTo* actionMove2 = CCMoveTo::create(duration, ccp(
                                                          pLabel->getPositionX(),
                                                          origin.y + visibleSize.height*3/4));
        
    pLabel->runAction(
                   CCSequence::create(
                                      actionMove1,
                                      actionMove2,
                                      NULL
                                      )
                   );
}

void GameResult::showScore()
{
    SimpleAudioEngine::sharedEngine()->playEffect("sound/gameover.mp3");

    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    CCLabelTTF* pLabel1 = CCLabelTTF::create("SCORE:", "Arial", 40);
    
    pLabel1->setPosition(ccp(origin.x + visibleSize.width*1/4,
                             origin.y + visibleSize.height*1/4));
    pLabel1->setTag(3);
    
    CCSprite* pSprite1 = CCSprite::create("skull1.png");
    pSprite1->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    pSprite1->setOpacity(64);
    pSprite1->setScale(0.5f);
    pSprite1->setTag(4);

    this->addChild(pLabel1, 1);
    this->addChild(pSprite1, 1);
}
