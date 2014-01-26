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
    
    void createCCSpriteBatchNode(const char* fileName);
    void addCharacter(CharacterBase* pCB);
    void addFloor(CCSprite* ccSprite);
    void update(float delta);
    
    CCSpriteBatchNode* getCCSpriteBatchNode() { return _ccSpriteBatchNode; }
    
private:
    CCSpriteBatchNode*          _ccSpriteBatchNode;
    std::vector<CharacterBase*> _character;
    std::vector<CCSprite*>      _floor;
};


#endif // CHARACTER_MANAGER_H
