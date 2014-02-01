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
    float testGameTime;
    
protected:
    
private:
    std::vector<std::map<std::string,int> > mapList;
    const char* boardImgName = "board.png";
    const char* backGroundImgName = "bg.png";
    CCNode* parentNode1;
    CCNode* parentNode2;
    CCSprite* bg1;
    CCSprite* bg2;

    int lastValue = 999;
    
    std::vector<std::map<std::string,int> > readMapFile();
    void initMap();
    void moveMap(float amount);
    
    void testScheduleMethod(float fDelta);
    
    void generateMap(CCNode* parentNode);
    
    
};

#endif /* defined(__GAME_SCENE__) */
