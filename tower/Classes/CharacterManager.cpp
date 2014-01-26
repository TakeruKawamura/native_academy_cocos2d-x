//
//  CharacterManager.cpp
//  group_a
//
//  Created by cocos2d-x on 14/01/26.
//
//

#include "CharacterManager.h"

CharacterManager::CharacterManager() {
    _ccSpriteBatchNode = NULL;
}


CharacterManager::~CharacterManager() {
    CharacterBase* cb = NULL;
    const int size = _character.size();
    
    if (size > 0) {
        for (int i=0; i<size; ++i) {
            cb = _character[i];
            
            if (cb != NULL) {
                delete cb;
            }
        }
        _character.clear();
    }
    
    _floor.clear(); // 参照しているだけなので大丈夫なはず
    
    if (_ccSpriteBatchNode != NULL) {
        _ccSpriteBatchNode->removeAllChildrenWithCleanup(true);
        
        _ccSpriteBatchNode->release();
        _ccSpriteBatchNode = NULL;
    }
}

void CharacterManager::createCCSpriteBatchNode(const char* fileName) {
    if (fileName != NULL && _ccSpriteBatchNode == NULL) {
        _ccSpriteBatchNode = CCSpriteBatchNode::create(fileName);
        
        _ccSpriteBatchNode->retain();
    }
}

void CharacterManager::addCharacter(CharacterBase* pCB) {
    if (pCB != NULL) {
        _character.push_back(pCB);
        
        CCSprite* spr = pCB->getCCSprite();
        
        // キャラクター描画は本クラスで行う
        if (_ccSpriteBatchNode != NULL && spr != NULL) {
            _ccSpriteBatchNode->addChild(spr);
        }
    }
}

void CharacterManager::addFloor(CCSprite* ccSprite) {
    // 当たり判定用の参照のみ
    _floor.push_back(ccSprite);
}

void CharacterManager::update(float delta) {
    CharacterBase* cb = NULL;
    const int size = _character.size();
    
    if (size > 0) {
        for (int i=0; i<size; ++i) {
            cb = _character[i];
            
            if (cb != NULL) {
                cb->update(delta);
            }
        }
    }
}