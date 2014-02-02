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

class CharacterManager;

class GameScene : public cocos2d::CCLayer {
public:
    GameScene();
    virtual ~GameScene();
    
    virtual bool init();
    static CCScene* scene();
    CREATE_FUNC(GameScene);
    float testGameTime;
    
    // =====================================================================
    // For Character
    void startGame();
    void update(float dt);
    
    void registerWithTouchDispatcher();
    bool ccTouchBegan( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent );
    // =====================================================================
    
protected:
    
private:
    std::vector<std::map<std::string,int> > mapList;
    const char* boardImgName = "board.png";
    const char* backGroundImgName = "bg.png";
    const char* jumpSound = "sound/jump.mp3";
    const char* dieSound = "sound/die.mp3";
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
    
    // =====================================================================
    // For Character
    const char*             PLAYER_L_PNG  = "player_l.png";
    const char*             PLAYER_R_PNG  = "player_r.png";
    const char*             ENEMY_0_L_PNG = "enemy0_l.png";
    const char*             ENEMY_0_R_PNG = "enemy0_r.png";
    const char*             ENEMY_1_L_PNG = "enemy1_l.png";
    const char*             ENEMY_1_R_PNG = "enemy1_r.png";
    
    bool initCharacter(const float backgroundOffsetPixcel);
    
    CharacterManager*       _characterManager;
    bool                    _start;
    // =====================================================================
};

#endif /* defined(__GAME_SCENE__) */
