#ifndef __GAME_RESULT_LOADER__
#define __GAME_RESULT_LOADER__

#include "GameResult.h"

class GameResultLoader : public cocos2d::extension::CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(GameResultLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(GameResult);
};

#endif // __GAME_RESULT_LOADER__
