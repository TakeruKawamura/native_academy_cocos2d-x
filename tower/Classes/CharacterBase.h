//
//  CharacerBase.h
//  group_a
//
//  Created by cocos2d-x on 14/01/26.
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
    
    void createCCSprite(CCTexture2D* ccTexture2D, const CCRect& ccRectRight, const CCRect& ccRectLeft);
    void update(float delta);
    
    CCSprite* getCCSprite() { return _ccSprite; }
    void setVisible(bool visible);
    void setPosition(const float x, const float y) { _x = x; _y = y; }
    void setFloor(CCSprite* ccSprite) { _ccSprite = ccSprite; }
    
protected:
    // 継承先で呼べるように
    void updateOnFloor(float delta);
    void updateFalling(float delta);
    
private:
    const float FRAME_PER_SECOND = 60.0f;
    const float DEGREE_180       = 180.0f;
    // スケール1倍時の横移動量
    const float WIDTH_MOVEMENT = 5.0f;
    const float GRAVITY_COEFFI = -9.8f;
    // GRAVITY_COEFFIを直接変えてもよかったが、分かりやすいように物理処理用のピクセル画面用係数を別途用意
    const float ADJUST_FACTOR_FOR_PHYSIC = 0.2f;
    
    void setDirectionLeft(bool left);
    bool calcFloor(float& leftX, float& rightX, float y);
    
    CCSprite*           _floor; // 参照のみ
    
    CCSprite*           _ccSprite;
    CCRect*             _texRectRight;
    CCRect*             _texRectLeft;
    float               _x;
    float               _y;
    float               _x0;
    float               _y0;
    float               _vx;
    float               _vy;
    float               _scale;
    float               _movement;
    float               _fallDelta;
    float               _screenWidth;
    bool                _directionLeft;
    bool                _turnFloor;
};

#endif // CHARACTER_BASE_H

