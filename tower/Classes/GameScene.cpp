//
//  GameScene.cpp
//  nyanshuffle
//
//  Created by cocos2d-x on 2014/01/13.
//
//

#include "GameScene.h"
#include "Animation.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

bool GameScene::init() {
    if (!CCLayer::init()) {
        return false;
    }
    
    srand((unsigned int)time(NULL));
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    //set background
    CCSprite* pBG = CCSprite::create("bg.png");
    pBG->setPosition(ccp(winSize.width * 0.5, winSize.height * 0.5));
    this->addChild(pBG);
    
    CCSize bgSize = pBG->getContentSize();
    
    return true;
}

CCScene* GameScene::scene() {
    CCScene* scene = CCScene::create();
    GameScene* layer = GameScene::create();
    scene->addChild(layer);
    
    return scene;
}