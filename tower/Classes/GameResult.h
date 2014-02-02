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
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    void addGameover();
    void moveGameover();
    void showScore();
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(GameResult);
};

#endif // __GAME_RESULT__
