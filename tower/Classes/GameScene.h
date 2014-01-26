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
    std::vector<std::map<std::string,int> > mapList;
    std::vector<CCSprite*> mapSpriteList;
    const char* boardImgName = "board.png";
    const char* backGroundImgName = "bg.png";
    
    std::vector<std::map<std::string,int> > readMapFile();
    void initMap(CCNode* parentNode);
    void moveMap(float amount);
    
};

#endif /* defined(__GAME_SCENE__) */
