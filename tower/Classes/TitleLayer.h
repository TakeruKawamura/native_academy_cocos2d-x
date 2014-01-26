#ifndef __tower__TitleLayer__
#define __tower__TitleLayer__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class TitleLayer : public CCLayer, public CCBSelectorResolver
{
        virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName);
        virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName);
    
        void tappedStart(CCObject* pSender, CCControlEvent pCCControlEvent);
        void tappedResult(CCObject* pSender, CCControlEvent pCCControlEvent);
    
    public:
        CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(TitleLayer, create);
};

#endif /* defined(__tower__TitleLayer__) */