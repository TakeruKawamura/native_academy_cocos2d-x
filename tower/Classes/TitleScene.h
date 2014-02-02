/*
 * 担当者：北島 領人
 */

#ifndef __group_a__TitleScene__
#define __group_a__TitleScene__

#include "cocos2d.h"

using namespace cocos2d;

class TitleScene : public cocos2d::CCLayer
{
public:
    virtual bool init();
    static cocos2d::CCScene* scene();
    CREATE_FUNC(TitleScene);
};

#endif /* defined(__group_a__TitleScene__) */
