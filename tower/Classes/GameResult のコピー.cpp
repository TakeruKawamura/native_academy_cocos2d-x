//
//  GameResult.cpp
//  tower
//
//  Created by cocos2d-x on 14/01/26.
//
//  Author Takeru Kawamura
//

#include "GameResult.h"

USING_NS_CC;

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

// on "init" you need to initialize your instance
bool GameResult::init()
{
    CCLOG("call init");
    
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
    
    // add a "close" icon to exit the progress. it's an autorelease object
    /*
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                                          "skull1.png",
                                                          "skull2.png",
                                                          this,
                                                          menu_selector(GameResult::menuCloseCallback));
    
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/10,
                                origin.y + pCloseItem->getContentSize().height/10));
    
    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);
    */
    /////////////////////////////
    // 3. add your codes below...
    
    // add a label shows "Hello World"
    // create and initialize a label
    /*
    CCLabelTTF* pLabel = CCLabelTTF::create("GAME OVER", "Arial", 50);
    
    // position the label on the center of the screen
    pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height*3/4));
    
    // add the label as a child to this layer
    this->addChild(pLabel, 1);
    */
    // add "HelloWorld" splash screen"
    CCSprite* pSprite1 = CCSprite::create("grave.png");
    
    // position the sprite on the center of the screen
    pSprite1->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    
    // add the sprite as a child to this layer
    this->addChild(pSprite1, 0);

    this->addGameover();
    this->scheduleOnce(schedule_selector(GameResult::showScore), 2.0f);
    
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
                                      //GameResult::showScore(),
                                      NULL
                                      )
                   );
    //this->schedule(schedule_selector(GameResult::moveGameover));
}

void GameResult::moveGameover()
{
    CCLOG("call moveGameover");
    
    //ここに記入されたモノを、定期的に呼び出す
}

void GameResult::showScore()
{
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

void GameResult::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}

