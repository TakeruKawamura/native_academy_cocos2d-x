#include "TitleLayer.h"

SEL_MenuHandler TitleLayer::onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "tappedStart", TitleLayer::tappedStart);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "tappedResult", TitleLayer::tappedResult);
    
    return NULL;
}

void TitleLayer::tappedStart(CCObject* pSender, CCControlEvent pCCControlEvent)
{
    CCLOG("start");
}

void TitleLayer::tappedResult(CCObject* pSender, CCControlEvent pCCControlEvent)
{
    CCLOG("result");
}

SEL_CCControlHandler TitleLayer::onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName)
{
    return NULL;
}
