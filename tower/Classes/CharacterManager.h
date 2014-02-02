//
//  CharacterManager.h
//
//  Created by Michihito Manaka on 14/01/26.
//
//
#ifndef CHARACTER_MANAGER_H
#define CHARACTER_MANAGER_H

#include "cocos2d.h"
#include "CharacterBase.h"
#include <vector>

USING_NS_CC;

class CharacterManager {
public:
    CharacterManager();
    virtual ~CharacterManager();
    
    //void createCCSpriteBatchNode(const char* fileName);
    void addCharacter(CharacterBase* pCB);
    void addFloor(CCSprite* ccSprite);
    void update(const float delta);
    void setVisible(const bool visible);
    void setJump();
    void setOffset(const float offsetX, const float offsetY) { _widthOffset = offsetX; _heightOffset = offsetY; }
    
    //CCSpriteBatchNode* getCCSpriteBatchNode() { return _ccSpriteBatchNode; }
    void setSprite(CCLayer* ccLayer);
    
private:
    bool checkCollisionFoor(CharacterBase* chara, CCSprite** outGetFloor);
    bool checkCollisionRect(CharacterBase* chara, CCSprite* floor);
    
    //CCSpriteBatchNode*          _ccSpriteBatchNode;
    std::vector<CharacterBase*> _character;
    std::vector<CCSprite*>      _floor;
    float                       _screenHeight;
    float                       _widthOffset; // 背景座標のオフセット用
    float                       _heightOffset;
};


#endif // CHARACTER_MANAGER_H
