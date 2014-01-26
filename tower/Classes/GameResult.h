//
//  GameResult.h
//  tower
//
//  Created by cocos2d-x on 14/01/26.
//
//

#ifndef __GAME_RESULT__
#define __GAME_RESULT__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

/*
 , public CCBMemberVariableAssigner
 */
class GameResult : public CCLayer
, public CCBSelectorResolver
{
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName);
    virtual bool init();

public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(GameResult, create);
};

#endif /* __GAME_LAYER__ */
