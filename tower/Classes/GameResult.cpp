//
//  GameResult.cpp
//  tower
//
//  Created by cocos2d-x on 14/01/26.
//
//

#include "GameResult.h"

SEL_MenuHandler GameResult::onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "init", GameResult::init);
    return NULL;
}

bool GameResult::init()
{
    CCLOG("init called");
}

SEL_CCControlHandler GameResult::onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName)
{
    return NULL;
}
