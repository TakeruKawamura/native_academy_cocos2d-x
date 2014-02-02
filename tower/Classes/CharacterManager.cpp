//
//  CharacterManager.cpp
//  group_a
//
//  Created by cocos2d-x on 14/01/26.
//
//

#include "CharacterManager.h"

CharacterManager::CharacterManager() {
    //_ccSpriteBatchNode = NULL;
    _screenHeight = 0.0f;
}


CharacterManager::~CharacterManager() {
    const int size = _character.size();
    
    if (size > 0) {
        for (int i=0; i<size; ++i) {
            CharacterBase* cb = _character[i];
            
            if (cb != NULL) {
                delete cb;
            }
            _character[i] = NULL;
        }
        _character.clear();
    }
    
    _floor.clear(); // 参照しているだけなので大丈夫なはず
    
    /*
    if (_ccSpriteBatchNode != NULL) {
        _ccSpriteBatchNode->removeAllChildrenWithCleanup(true);
        
        _ccSpriteBatchNode->release();
        _ccSpriteBatchNode = NULL;
    }
    */
}

/*
void CharacterManager::createCCSpriteBatchNode(const char* fileName) {
    if (fileName != NULL && _ccSpriteBatchNode == NULL) {
        _ccSpriteBatchNode = CCSpriteBatchNode::create(fileName);
        
        _ccSpriteBatchNode->retain();
    }
}
*/

void CharacterManager::setSprite(CCLayer* ccLayer) {
    if (ccLayer == NULL) return;
    
    const int size = _character.size();
    
    if (size > 0) {
        for (int i=0; i<size; ++i) {
            CharacterBase* cb = _character[i];
            
            if (cb != NULL) {
                ccLayer->addChild(cb->getCCSpriteL());
                ccLayer->addChild(cb->getCCSpriteR());
            }
        }
    }
    
    // Initとかないのでここで処理(setSpriteは一回だけ呼ばれる前提なので)
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    _screenHeight = visibleSize.height;
}

void CharacterManager::addCharacter(CharacterBase* pCB) {
    if (pCB != NULL) {
        _character.push_back(pCB);
        
        /*
        CCSprite* sprL = pCB->getCCSpriteL();
        CCSprite* sprR = pCB->getCCSpriteR();
        
        // キャラクター描画は本クラスで行う
        if (_ccSpriteBatchNode != NULL && sprL != NULL && sprR != NULL) {
            _ccSpriteBatchNode->addChild(sprL);
            _ccSpriteBatchNode->addChild(sprR);
        }
        */
    }
}

void CharacterManager::addFloor(CCSprite* ccSprite) {
    // 当たり判定用の参照のみ
    _floor.push_back(ccSprite);
}

void CharacterManager::setJump() {
    const int size = _character.size();
    
    if (size > 0) {
        for (int i=0; i<size; ++i) {
            CharacterBase* cb = _character[i];
            
            if (cb != NULL) {
                // 床をターンするタイプ(プレイヤー)だけジャンプ
                if (cb->isTurnFloor()) {
                    cb->setJump();
                }
            }
        }
    }
}

void CharacterManager::update(const float delta) {
    const int size = _character.size();
    
    if (size > 0) {
        for (int i=0; i<size; ++i) {
            CharacterBase* cb = _character[i];
            
            if (cb != NULL) {
                cb->update(delta);
                
                // キャラのアップデート後に床に乗っていない、ジャンプ上昇中でなければコリジョン処理を行う
                if (!cb->isFloor() && !cb->isRise()) {
                    // 追加 : 画面外なら処理しない
                    float y = cb->getY();
                    
                    // スプライトに中心Yだけど気にしない
                    if (y >= 0.0f && y <= _screenHeight) {
                        CCSprite* onFloor = NULL;
                        
                        checkCollisionFoor(cb, &onFloor);
                        cb->setOnFloor(onFloor); // NULLセットが床上にない判定
                    }
                }
            }
        }
    }
}

bool CharacterManager::checkCollisionFoor(CharacterBase* chara, CCSprite** outGetFloor) {
    *outGetFloor = NULL;
    
    if (chara == NULL) return false;
    
    // 既に床の上
    if (chara->isFloor()) return false;
    
    const int floorNum = _floor.size();
    
    if (floorNum <= 0) return false;
    
    // 本来なら空間分割管理等するべきだが、今回は全検索で
    for (int i=0; i<floorNum; ++i) {
        CCSprite* spr = _floor[i];
        
        if (spr != NULL) {
            if (checkCollisionRect(chara, spr)) {
                // 複数は考慮しない、最初に判定したものを返す
                *outGetFloor = spr;
                return true;
            }
        }
    }
    
    return false;
}

bool CharacterManager::checkCollisionRect(CharacterBase* chara, CCSprite* floor) {

    CCSprite* charaSpr  = chara->getCCSprite();
    CCRect    charaRect = charaSpr->boundingBox();
    CCRect    floorRect = floor->boundingBox();
    
#if false
    // 通り抜けは考慮しない
    return floorRect.intersectsRect(charaRect);
#else
    // 幅チェック
    const float floorXL    = floorRect.origin.x;
    const float floorXR    = floorRect.origin.x + floorRect.size.width;

    const float charaXL    = charaRect.origin.x;
    const float charaXR    = charaRect.origin.x + charaRect.size.width;
    
    // キャラの左端が床の右端より大きい、キャラの右端が床の左端より小さい
    if (charaXL > floorXR || charaXR < floorXL) return false;
    
    // 高さチェック(落下している前提なので、キャラクターの前回のY値も使う)
    const float floorY    = floorRect.origin.y + floorRect.size.height;
    const float charaOldY = chara->getOldY();
    const float charaY    = charaRect.origin.y;
    
    // 前回のキャラの下端が床位置より高く、現在のキャラの下端が床位置より低いなら通貨した
    if (floorY >= charaY && floorY <= charaOldY) {
        return true;
    }
    
    return false;
#endif
}

void CharacterManager::setVisible(const bool visible) {
    CharacterBase* cb = NULL;
    const int size = _character.size();
    
    if (size > 0) {
        for (int i=0; i<size; ++i) {
            cb = _character[i];
            
            if (cb != NULL) {
                cb->setVisible(visible);
            }
        }
    }
}