//
//  CharacerBase.cpp
//  group_a
//
//  Created by cocos2d-x on 14/01/26.
//
//

#include "CharacterBase.h"
#include "math.h"

CharacterBase::CharacterBase() {
    _floor            = NULL;
    _ccSpriteL        = NULL;
    _ccSpriteR        = NULL;
    _x                = 0.0f;
    _y                = 0.0f;
    _oldy             = 0.0f;
    _x0               = 0.0f;
    _y0               = 0.0f;
    _vx               = 0.0f;
    _vy               = 0.0f;
    _fallDelta        = 0.0f;
    _screenWidthStart = 0.0f;
    _screenWidthEnd   = 0.0f;
    _directionLeft    = false;
    _turnFloor        = false;
    _rise             = false;
    _jump             = false;
}

CharacterBase::~CharacterBase() {
    if (_ccSpriteR != NULL) {
        _ccSpriteR->release();
        _ccSpriteR = NULL;
    }
    
    if (_ccSpriteL != NULL) {
        _ccSpriteL->release();
        _ccSpriteL = NULL;
    }
}

//void CharacterBase::createCCSprite(CCTexture2D* ccTexture2D,
//                                   const CCRect& ccRectRight, const CCRect& ccRectLeft,
void CharacterBase::createCCSprite(const char* fileNameR, const char* fileNameL,
                                                        const float backgroundOffsetPixcel) {
    //if (_ccSpriteL == NULL && _ccSpriteR == NULL && ccTexture2D != NULL) {
    if (fileNameR != NULL && fileNameL != NULL) {
        // 始めは左向き
        _directionLeft = false;
        
        //_ccSpriteL = CCSprite::createWithTexture(ccTexture2D, ccRectLeft);
        _ccSpriteL = CCSprite::create(fileNameL);
        _ccSpriteL->retain();
        
        // 始めは表示しない
        _ccSpriteL->setVisible(false);
        
        //_ccSpriteR = CCSprite::createWithTexture(ccTexture2D, ccRectRight);
        _ccSpriteR = CCSprite::create(fileNameR);
        _ccSpriteR->retain();
        
        _ccSpriteR->setVisible(false);
        
        // 途中画面サイズ変更は想定しない
        CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
        
        _screenWidthStart = backgroundOffsetPixcel;
        _screenWidthEnd   = visibleSize.width - backgroundOffsetPixcel;
        
        _vx = 0.0f;
        _vy = -WIDTH_MOVEMENT;
    }
}

void CharacterBase::setVisible(const bool visible) {
    CCSprite* target = getCCSprite();
    
    if (target != NULL) {
        target->setVisible(visible);
    }
}

void CharacterBase::setDirectionLeft(const bool left) {
    _directionLeft = left;
    
    if (left) {
        _ccSpriteR->setPosition(ccp(_x, _y));
        _ccSpriteR->setVisible(false);
        _ccSpriteL->setVisible(true);
        _ccSpriteL->setPosition(ccp(_x, _y));
    }
    else {
        _ccSpriteL->setPosition(ccp(_x, _y));
        _ccSpriteL->setVisible(false);
        _ccSpriteR->setVisible(true);
        _ccSpriteR->setPosition(ccp(_x, _y));
    }
}

void CharacterBase::update(const float delta) {
    // 更新不要
    if (_ccSpriteR == NULL || _ccSpriteL == NULL) return;
    
    // 床のスプライトがないなら床上にいない仕様とする
    if (_floor == NULL) {
        updateFalling(delta);
    }
    else {
        updateOnFloor(delta);
    }
    
    CCSprite* target = getCCSprite();
    
    // 一つ前のY座標を保存(落下する前提なのでコリジョン用)
    _oldy = target->boundingBox().origin.y;
    
    target->setPosition(ccp(_x, _y));
    
    // ジャンプ中
    if (_jump && _rise) {
        float y = target->boundingBox().origin.y;
        
        if (y < _oldy) {
            _rise = false;
        }
    }
}

bool CharacterBase::calcFloor(float& leftX, float& rightX, float& y) {
    leftX  = 0.0f;
    rightX = 0.0f;
    y      = 0.0f;
    
    bool ret = false;
    
    if (_floor != NULL) {
        CCRect bound = _floor->boundingBox();
        
        leftX  = bound.origin.x;
        rightX = bound.origin.x + bound.size.width;
        y      = bound.origin.y + bound.size.height;
        
        ret = true;
    }
    
    return ret;
}

void CharacterBase::updateOnFloor(const float delta) {
    // 1.0fを基準にした値に変換する
    const float deltaCoeffi = delta * FRAME_PER_SECOND;
    
    // 処理は右下基準で行う
    CCRect rect = getCCSprite()->boundingBox();
    
    float movement = WIDTH_MOVEMENT * deltaCoeffi;
    
    if (_directionLeft) {
        movement = -movement;
    }
    
    const float sprHalfW = rect.size.width * 0.5f;
    
    // 移動後のコリジョン座標
    float movedX = rect.origin.x + movement;
    
    // BGの両端を考慮する
    if (movedX < _screenWidthStart) {
        setDirectionLeft(false);
        _x = _screenWidthStart + sprHalfW + REVERSE_OFFSET;
    }
    else if (movedX > _screenWidthEnd) {
        setDirectionLeft(true);
        _x = _screenWidthEnd - sprHalfW - REVERSE_OFFSET;
    }
    else {
        // 移動後の描画X座標
        movedX += sprHalfW;
        
        // 床上チェック
        float left, right, y;
        
        if (calcFloor(left, right, y)) {
            // 高さはセットしてしまう
            // 当たり判定はRect.originで行っているので、描画座標はキャラ高の半分を足す
            _y = y + rect.size.height * 0.5f;
            
            // 向きによる判定
            if (_directionLeft) {
                if (movedX + sprHalfW < left) {
                    if (_turnFloor) {
                        setDirectionLeft(false);
                        _x = left - sprHalfW + REVERSE_OFFSET;
                    }
                    else {
                        _x = movedX;
                        _floor = NULL;
                    }
                }
                else {
                    _x = movedX;
                }
            }
            else {
                if (movedX - sprHalfW > right) {
                    if (_turnFloor) {
                        setDirectionLeft(true);
                        _x = right + sprHalfW - REVERSE_OFFSET;
                    }
                    else {
                        _x = movedX;
                        _floor = NULL;
                    }
                }
                else {
                    _x = movedX;
                }
            }
        }
    }
    
    if (_floor == NULL) {
        // 落下時間初期化
        _fallDelta = 0.0f;
        
        // 落下開始位置を保存
        _x0 = _x;
        _y0 = _y;
        
        // 初速をセット
        float degree = 0.0f;
        
        if (_directionLeft) {
            degree   = 240.0f;
            
        }
        else {
            degree   = -60.0f;
        }
        
        float theta = degree * M_PI / DEGREE_180;
        
        _vx = movement * cosf(theta);
        _vy = movement * sinf(theta);
    }
}

void CharacterBase::updateFalling(const float delta) {
    // 1.0fを基準にした値に変換する
    const float deltaCoeffi = delta * FRAME_PER_SECOND;
    
    _fallDelta += deltaCoeffi;
    
    // P = P0 + V0*time + 1/2*G*Time^2
    _x = _x0 + (_vx * _fallDelta);
    _y = _y0 + (_vy * _fallDelta) + (0.5f * GRAVITY_COEFFI * _fallDelta * _fallDelta * ADJUST_FACTOR_FOR_PHYSIC);
}

void CharacterBase::setJump() {
    if (_floor != NULL) {
        _floor = NULL;
        _jump = true;
        _rise = true;
        
        // 落下時間初期化
        _fallDelta = 0.0f;
        
        // 落下開始位置を保存
        _x0 = _x;
        _y0 = _y;
        
        // 初速をセット
        float movement = 0.0f;
        float degree = 0.0f;
        
        if (_directionLeft) {
            movement = -WIDTH_MOVEMENT;
            degree   = 120.0f;
            
        }
        else {
            movement = WIDTH_MOVEMENT;
            degree   = 60.0f;
        }
        
        float theta = degree * M_PI / DEGREE_180;
        
        _vx = movement * cosf(theta);
        _vy = movement * sinf(theta);
    }
}

