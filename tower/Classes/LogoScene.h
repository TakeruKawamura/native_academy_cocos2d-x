/*
 * 担当者：北島 領人
 */

#ifndef __group_a__LogoScene__
#define __group_a__LogoScene__

#include "cocos2d.h"

using namespace cocos2d;

class LogoScene : public cocos2d::CCLayer
{
    public:
        virtual bool init();
        void nextScene();
        static cocos2d::CCScene* scene();
        CREATE_FUNC(LogoScene);
};

#endif /* defined(__group_a__LogoScene__) */
