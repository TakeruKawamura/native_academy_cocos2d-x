//
//  GameScene.h
//  nyanshuffle
//
//  Created by cocos2d-x on 2014/01/13.
//
//

#ifndef __GAME_SCENE__
#define __GAME_SCENE__

#include "cocos2d.h"

USING_NS_CC;

class GameScene : public cocos2d::CCLayer {
public:
    virtual bool init();
    static CCScene* scene();
    CREATE_FUNC(GameScene);
    
protected:
    
private:
    
};

#endif /* defined(__GAME_SCENE__) */
