/*
 * 担当者：北島 領人
 */

#ifndef __tower__TitleLayerLoader__
#define __tower__TitleLayerLoader__

#include "TitleLayer.h"

class TitleLayerLoader : public cocos2d::extension::CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(TitleLayerLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(TitleLayer);
};

#endif