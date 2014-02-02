//
//  CharacterManager.h
//  group_a
//
//  Created by cocos2d-x on 14/01/26.
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
    
    //CCSpriteBatchNode* getCCSpriteBatchNode() { return _ccSpriteBatchNode; }
    void setSprite(CCLayer* ccLayer);
    
private:
    bool checkCollisionFoor(CharacterBase* chara, CCSprite** outGetFloor);
    bool checkCollisionRect(CharacterBase* chara, CCSprite* floor);
    
    //CCSpriteBatchNode*          _ccSpriteBatchNode;
    std::vector<CharacterBase*> _character;
    std::vector<CCSprite*>      _floor;
};


#endif // CHARACTER_MANAGER_H
