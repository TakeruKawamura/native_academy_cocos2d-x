//
//  GameResult.h
//  tower
//
//  Created by cocos2d-x on 14/01/26.
//
//  Author Takeru Kawamura
//

#ifndef __GAME_RESULT__
#define __GAME_RESULT__

#include "cocos2d.h"
#include "cocos-ext.h"

class GameResult : public cocos2d::CCLayer
{
public:
    virtual bool init();
    void addGameover();
    void showScore();
    
    static cocos2d::CCScene* scene();
    
    void menuCloseCallback(CCObject* pSender);
    
    CREATE_FUNC(GameResult);
};

#endif // __GAME_RESULT__
