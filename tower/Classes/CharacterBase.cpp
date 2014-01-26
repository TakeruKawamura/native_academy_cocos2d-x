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
    _ccSprite         = NULL;
    _texRectRight     = NULL;
    _texRectLeft      = NULL;
    _x                = 0.0f;
    _y                = 0.0f;
    _x0               = 0.0f;
    _y0               = 0.0f;
    _vx               = 0.0f;
    _vy               = 0.0f;
    _scale            = 1.0f;
    _movement         = WIDTH_MOVEMENT;
    _fallDelta        = 0.0f;
    _screenWidth      = 0.0f;
    _directionLeft    = false;
    _turnFloor        = false;
}

CharacterBase::~CharacterBase() {
    if (_ccSprite != NULL) {
        // いるのか？
        //_ccSprite->release();
        _ccSprite = NULL;
    }
    
    if (_texRectLeft != NULL) {
        delete _texRectLeft;
        _texRectLeft = NULL;
    }
    
    if (_texRectRight != NULL) {
        delete _texRectRight;
        _texRectRight = NULL;
    }
}

void CharacterBase::createCCSprite(CCTexture2D* ccTexture2D,
                                   const CCRect& ccRectRight, const CCRect& ccRectLeft) {
    if (_ccSprite == NULL && _texRectRight == NULL && _texRectLeft == NULL && ccTexture2D != NULL) {
        _texRectRight = new CCRect(ccRectRight);
        _texRectLeft  = new CCRect(ccRectLeft);
        
        // 始めは左向き
        _ccSprite = CCSprite::createWithTexture(ccTexture2D, *(_texRectLeft));
        _directionLeft = true;
        
        // 下辺の中央がアンカーポイント
        _ccSprite->setAnchorPoint(ccp(0.0f, 0.5f));
        
        // 始めは表示しない
        _ccSprite->setVisible(false);
        
        // スケール(画面のアスペクトは維持で、描画スケールのみ変更の仕様)
        float scale = _ccSprite->getScale();
        
        _scale    = scale;
        _movement = WIDTH_MOVEMENT * scale;
        
        // 途中画面サイズ変更は想定しない
        CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
        
        _screenWidth = visibleSize.width;
    }
}

void CharacterBase::setVisible(bool visible) {
    if (_ccSprite != NULL) {
        _ccSprite->setVisible(visible);
    }
}

void CharacterBase::setDirectionLeft(bool left) {
    if (left) {
        if (!_directionLeft) {
            _ccSprite->setTextureRect(*(_texRectLeft));
            _directionLeft = true;
        }
    }
    else {
        if (_directionLeft) {
            _ccSprite->setTextureRect(*(_texRectRight));
            _directionLeft = false;
        }
    }
}

bool CharacterBase::calcFloor(float& leftX, float& rightX, float y) {
    leftX  = 0.0f;
    rightX = 0.0f;
    y      = 0.0f;
    
    bool ret = false;
    
    if (_floor != NULL) {
        CCRect bound = _floor->boundingBox();
        
        leftX  = bound.origin.x;
        rightX = bound.origin.x + bound.size.width;
        y      = bound.origin.y;
        
        ret = true;
    }
    
    return ret;
}

void CharacterBase::update(float delta) {
    // 更新不要
    if (_ccSprite == NULL) return;
    
    // 床のスプライトがないなら床上にいない仕様とする
    if (_floor == NULL) {
        updateFalling(delta);
    }
    else {
        updateOnFloor(delta);
    }
    
    _ccSprite->setPosition(ccp(_x, _y));
}

void CharacterBase::updateOnFloor(float delta) {
    // 1.0fを基準にした値に変換する
    float deltaCoeffi = delta * FRAME_PER_SECOND;
    
    // スプライトの幅(setAnchorPoint(ccp(0.0f, 0.5f))しているので x 0.5f)
    float checkX = _ccSprite->boundingBox().size.width * 0.5f;
    
    float movement = 0.0f;
    
    if (_directionLeft) {
        movement = -_movement * deltaCoeffi;
    }
    else {
        movement = _movement * deltaCoeffi;
        checkX   = -checkX;
    }
    
    // 移動
    _x += movement;
    
    // 床上チェック
    float left, right, y;
    
    if (calcFloor(left, right, y)) {
        _y = y;
        
        float pointX = 0.0f;
        
        if (_turnFloor) {
            // ターンチェックはアンカーポイントで
            pointX = _x;
        }
        else {
            // 落下チェックは方向と逆に幅の半分を足して
            pointX = _x + checkX;
        }
        
        if (_directionLeft) {
            if (pointX < left) {
                if (_turnFloor) {
                    _x = left;
                    setDirectionLeft(false);
                }
                else {
                    _floor = NULL;
                }
            }
        }
        else {
            if (pointX > right) {
                if (_turnFloor) {
                    _x = right;
                    setDirectionLeft(true);
                }
                else {
                    _floor = NULL;
                }
            }
        }
    }
    
    // 画面端チェック
    if (_x < 0.0f) {
        _x = 0.0f;
        setDirectionLeft(false);
    }
    else if (_x > _screenWidth) {
        _x = _screenWidth;
        setDirectionLeft(true);
    }
    
    if (_floor == NULL) {
        // 落下開始位置を保存
        _x0 = _x;
        _y0 = _y;
        
        // 初速をセット
        // 2Dの回転(回転するのは_movementなのでyは0.0f)
        // x' = x * cosθ - y * sinθ
        // y' = x * sinθ + y * cosθ
        float degree = 0.0f, movement = 0.0f;
        
        if (_directionLeft) {
            movement = -movement;
            degree   = 225.0f;
            
        }
        else {
            movement = movement;
            degree   = -45.0f;
        }
        
        float theta = degree * M_PI / DEGREE_180;
        
        _vx = movement * cosf(theta);
        _vy = movement * sinf(theta);
        
        // 落下時間初期化
        _fallDelta = 0.0f;
    }
}

void CharacterBase::updateFalling(float delta) {
    _fallDelta += delta;
    
    // P = P0 + V0*time + 1/2*G*Time^2
    float coefficient = 0.5f * GRAVITY_COEFFI * _fallDelta * _fallDelta;
    
    coefficient *= _scale * ADJUST_FACTOR_FOR_PHYSIC;
    
    _x = _x0 + (_vx * _fallDelta) + coefficient;
    _y = _y0 + (_vy * _fallDelta) + coefficient;
}

