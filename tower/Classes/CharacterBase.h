//
//  CharacerBase.h
//
//  Created by Michihito Manaka on 14/01/26.
//
//
#ifndef CHARACTER_BASE_H
#define CHARACTER_BASE_H

#include "cocos2d.h"

USING_NS_CC;

class CharacterManager;

class CharacterBase {
public:
    CharacterBase();
    virtual ~CharacterBase();
    
    //void createCCSprite(CCTexture2D* ccTexture2D, const CCRect& ccRectRight, const CCRect& ccRectLeft, const float backgroundOffsetPixcel = 0.0f);
    void createCCSprite(const char* fileNameR, const char* fileNameL,
                            const bool firstVisible, const bool dirLeft,
                            const float backgroundOffsetPixcel = 0.0f);
    void update(const float delta);
    
    CCSprite* getCCSprite() { return _directionLeft ? _ccSpriteL : _ccSpriteR; }
    CCSprite* getCCSpriteL() { return _ccSpriteL; }
    CCSprite* getCCSpriteR() { return _ccSpriteR; }
    float getY() { return _y; }
    float getOldY() { return _oldy; }
    void setVisible(const bool visible);
    void setTurnFloor(const bool turn) { _turnFloor = turn; }
    void setJump() { _setJump = true; }
    void setPosition(const float x, const float y) { _x = x; _x0 = x; _y = y; _y0 = y; } // 落下初期位置にもセット
    void setOnFloor(CCSprite* ccSprite) { _floor = ccSprite; _rise = false; _jump = false; }
    bool isTurnFloor() { return _turnFloor; }
    bool isFloor() { return (_floor == NULL) ? false : true; }
    bool isRise() { return _rise ? true : false; }
    
protected:
    // 継承先で呼べるように
    void updateOnFloor(const float delta);
    void updateFalling(const float delta);
    
private:
    const float FRAME_PER_SECOND = 60.0f;
    const float DEGREE_180       = 180.0f;
    const float WIDTH_MOVEMENT   = 5.0f;
    const float JUMP_VELOCITY    = 7.5f;
    const float GRAVITY_COEFFI   = -9.8f;
    const float REVERSE_OFFSET   = 2.0f;
    const float ADJUST_FACTOR_FOR_PHYSIC = 0.005f;
    
    void setDirectionLeft(const bool left);
    bool calcFloor(float& leftX, float& rightX, float& y);
    void doJump();
    void onJump();
    void checkGround();
    
    CCSprite*           _floor;             // 参照のみ
    CCSprite*           _ccSpriteL;
    CCSprite*           _ccSpriteR;
    float               _x;
    float               _y;
    float               _oldy;              // 1つ前のCCRectのY値
    float               _x0;
    float               _y0;
    float               _vx;
    float               _vy;
    float               _fallDelta;
    float               _screenWidthStart;
    float               _screenWidthEnd;
    float               _screenHeight;
    bool                _directionLeft;
    bool                _turnFloor;         // 床の上で向きを変えるか(プレイヤーかどうか)
    bool                _ground;            // プレイヤーの地面処理
    bool                _rise;              // ジャンプ上昇中か
    bool                _jump;              // ジャンプ中か
    bool                _setJump;
};

#endif // CHARACTER_BASE_H

