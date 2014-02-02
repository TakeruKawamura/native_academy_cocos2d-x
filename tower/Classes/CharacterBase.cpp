//
//  CharacerBase.cpp
//
//  Created by Michihito Manaka on 14/01/26.
//
//

#include "GameScene.h"
#include "CharacterManager.h"
#include "CharacterBase.h"
#include "GameResult.h"
#include "math.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

CharacterBase::CharacterBase() {
    _manager          = NULL;
    _scene            = NULL;
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
    _screenHeight     = 0.0f;
    _floorOffsetX     = 0.0f;
    _floorOffsetY     = 0.0f;
    _floorOffsetAddY  = 0.0f;
    _directionLeft    = false;
    _turnFloor        = false;
    _ground           = false;
    _jump             = false;
    _setJump          = false;
    _firstOnFloor     = false;
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
                                    const bool firstVisible, const bool dirLeft,
                                            const float backgroundOffsetPixcel) {
    //if (_ccSpriteL == NULL && _ccSpriteR == NULL && ccTexture2D != NULL) {
    if (fileNameR != NULL && fileNameL != NULL) {
        //_ccSpriteR = CCSprite::createWithTexture(ccTexture2D, ccRectRight);
        _ccSpriteR = CCSprite::create(fileNameR);
        _ccSpriteR->retain();
        
        //_ccSpriteL = CCSprite::createWithTexture(ccTexture2D, ccRectLeft);
        _ccSpriteL = CCSprite::create(fileNameL);
        _ccSpriteL->retain();
        
        _directionLeft = dirLeft;
        
        
        _vy = -WIDTH_MOVEMENT;
        
        bool visibleR = false;
        bool visibleL = false;
        
        if (firstVisible) {
            if (_directionLeft) {
                visibleL = true;
            }
            else {
                visibleR = true;
            }
        }
        
        _ccSpriteR->setVisible(visibleR);
        _ccSpriteL->setVisible(visibleL);
        
        // 最初の落下用
        float degree = 0.0f;
        
        if (_directionLeft) {
            degree   = 240.0f;
            
        }
        else {
            degree   = -60.0f;
        }
        
        float theta = degree * M_PI / DEGREE_180;
        
        _vx = WIDTH_MOVEMENT * cosf(theta);
        _vy = WIDTH_MOVEMENT * sinf(theta);
        
        // 途中画面サイズ変更は想定しない
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        
        _screenWidthStart = backgroundOffsetPixcel;
        _screenWidthEnd   = winSize.width - backgroundOffsetPixcel;
        
        _screenHeight = winSize.height;
        
        //include music
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.5);
        SimpleAudioEngine::sharedEngine()->preloadEffect(dieSound);
        SimpleAudioEngine::sharedEngine()->preloadEffect(jumpSound);
        
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
    
    if (_setJump) {
        _setJump = false;
        doJump();
    }
    
    // 床のスプライトがないなら床上にいない仕様とする
    if (_floor == NULL && !_ground) {
        updateFalling(delta);
    }
    else {
        updateOnFloor(delta);
    }
    
    // 画面下端処理
    checkGround();
    
    // 一つ前のY座標を保存(落下する前提なのでコリジョン用)
    CCSprite* target = getCCSprite();
    
    _oldy = target->boundingBox().origin.y;
    target->setPosition(ccp(_x, _y));
    
    // ジャンプ中処理
    onJump();
}

void CharacterBase::onJump() {
    if (_jump && _rise) {
        const float y = getCCSprite()->boundingBox().origin.y;
        
        if (y < _oldy) {
            _rise = false;
        }
    }
}

void CharacterBase::setOnFloor(CCSprite* ccSprite) {
    // 一応ポインタ比較でチェック(変わった時点でセットしたい)
    if (_floor != ccSprite) {
        _floor = ccSprite;
        _rise = false;
        _jump = false;
        
        if (!_firstOnFloor) _firstOnFloor = true;
        
        checkBackGroundOffsetY();
    }
}

void CharacterBase::checkBackGroundOffsetY() {
    const float y = getCCSprite()->boundingBox().origin.y;
    
    const float halfHeight = _screenHeight * 0.5f;
    
    // 画面をスクロールするか
    if (y > halfHeight) {
        float diff = y - halfHeight;
        
        if (_scene != NULL) {
            _scene->moveMap(diff);
            
            addFloorOffsetY(-diff);
            
            if (_manager != NULL) {
                _manager->addOffsetY(-diff);
            }
        }
    }
}

void CharacterBase::checkGround() {
    CCSprite* target = getCCSprite();
    CCRect    rect   = target->boundingBox();
    
    // _yはアンカーなのでスプライト中心なのに留意
    if (_turnFloor) {
        // プレイヤーなら地面を歩けるようにする
        const float sprHalfH = rect.size.height * 0.5f;
        
        if (_y - sprHalfH <= 0.0f) {
            _y = sprHalfH;
            if (!_ground) _ground = true;
            
            if (_firstOnFloor) {
                setResultScene();
            }
        }
    }
    else {
        // プレイヤーでなければ画面上部に戻す
        const float sprH = rect.size.height;
        
        if (_y < -sprH) {
            _y = _screenHeight + sprH;
            _y0 = _y;
            
            _fallDelta = 0.0f;
        }
    }
}

void CharacterBase::setResultScene() {
    SimpleAudioEngine::sharedEngine()->playEffect(dieSound);
    
    CCScene* result = GameResult::scene();
    CCTransitionFade* tran = CCTransitionFade::create(2.0f, result, ccc3(0, 0, 0));
    CCDirector::sharedDirector()->replaceScene(tran);
}

bool CharacterBase::calcFloor(float& leftX, float& rightX, float& y) {
    leftX  = 0.0f;
    rightX = 0.0f;
    y      = 0.0f;
    
    bool ret = false;
    
    if (_floor != NULL) {
        CCRect bound = _floor->boundingBox();
        
        leftX  = bound.origin.x + _floorOffsetX;
        rightX = bound.origin.x + bound.size.width + _floorOffsetX;
        y      = bound.origin.y + bound.size.height + _floorOffsetY + _floorOffsetAddY;
        
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
    if (movedX <= _screenWidthStart) {
        setDirectionLeft(false);
        _x = _screenWidthStart + sprHalfW + REVERSE_OFFSET;
    }
    else if (movedX >= _screenWidthEnd) {
        setDirectionLeft(true);
        _x = _screenWidthEnd - sprHalfW - REVERSE_OFFSET;
    }
    else {
        // 移動後の描画X座標
        movedX += sprHalfW;
        
        // プレイヤーで地面にいるなら処理抜ける
        if (_turnFloor && _ground) {
            _x = movedX;
            return;
        }
        
        // 床上チェック
        float left, right, y;
        
        // _x, _yはスプライト中心(アンカー？)、CCRextは左下基準で幅高さなのに留意
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
    
    if (_floor == NULL && !_ground) {
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
        
        _vx = WIDTH_MOVEMENT * cosf(theta);
        _vy = WIDTH_MOVEMENT * sinf(theta);
    }
}

void CharacterBase::updateFalling(const float delta) {
    // 1.0fを基準にした値に変換する
    const float deltaCoeffi = delta * FRAME_PER_SECOND;
    
    _fallDelta += deltaCoeffi;
    
    // 物理計算結果をピクセル単位に調整する
    // * ADJUST_FACTOR_FOR_PHYSIC;
    
    // P = P0 + V0*time + 1/2*G*Time^2
    _x = _x0 + _vx * _fallDelta;
    _y = _y0 + (_vy * _fallDelta) + ((0.5f * GRAVITY_COEFFI * _fallDelta * _fallDelta) * ADJUST_FACTOR_FOR_PHYSIC);
    
    // 画面端処理
    if (_x <= _screenWidthStart) {
        setDirectionLeft(false);
        _x0 = _screenWidthStart - _x0 + REVERSE_OFFSET;
        _vx = -_vx;
    }
    else if (_x >= _screenWidthEnd) {
        setDirectionLeft(true);
        _x0 = _screenWidthEnd + (_screenWidthEnd - _x0 - REVERSE_OFFSET);
        _vx = -_vx;
    }
}

void CharacterBase::doJump() {
    if (_floor != NULL || _ground) {
        _floor = NULL;
        _jump = true;
        _rise = true;
        _ground = false;
        
        // 落下時間初期化
        _fallDelta = 0.0f;
        
        // 落下開始位置を保存
        _x0 = _x;
        _y0 = _y;
        
        // 初速をセット
        float degree = 0.0f;
        
        if (_directionLeft) {
            degree   = 120.0f;
            
        }
        else {
            degree   = 60.0f;
        }
        
        float theta = degree * M_PI / DEGREE_180;
        
        _vx = JUMP_VELOCITY * cosf(theta);
        _vy = JUMP_VELOCITY * sinf(theta);
    }
}

void CharacterBase::setJump() {
    SimpleAudioEngine::sharedEngine()->playEffect(jumpSound);
     _setJump = true;
}