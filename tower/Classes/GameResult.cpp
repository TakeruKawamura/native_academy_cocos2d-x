//
//  GameResult.cpp
//  tower
//
//  Created by cocos2d-x on 14/01/26.
//
//

#include "GameResult.h"

GameResult::GameResult()
: gameover(NULL)
{
    CCLOG("constructor");
}
GameResult::~GameResult()
{
    CCLOG("destructor");
    CC_SAFE_RELEASE(gameover);
}

bool GameResult::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "gameover", CCSprite*, this->gameover);

    return false;
}

SEL_MenuHandler GameResult::onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "init", GameResult::init);
    return NULL;
}

bool GameResult::init()
{
    if (!CCLayer::init())
    {
        return false;
    }
    CCLOG("init");
    this->scheduleUpdate();
    
    return true;
}

SEL_CCControlHandler GameResult::onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName)
{
    return NULL;
}

void GameResult::update(float delta){
    
    //ここに記入されたモノを、定期的に呼び出す
    CCLOG("schedule update");
    gameover->setPosition(ccp(gameover->getPositionX() + 100*delta, gameover->getPositionY()));
    
}
