/*
 * 担当者：北島 領人
*/

#include "LogoScene.h"
#include "TitleScene.h"

USING_NS_CC;

CCScene* LogoScene::scene()
{
    CCScene *scene = CCScene::create();
    LogoScene *layer = LogoScene::create();
    scene->addChild(layer);
    
    return scene;
}

bool LogoScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    // チームAロゴの生成
    CCSprite* logoTeamA = CCSprite::create("team_a.png");
    logoTeamA->setPosition(ccp(winSize.width * 0.5, winSize.height * 0.5));
    
    // チームAロゴのアニメーション設定
    CCActionInterval* fadeIn = CCFadeIn::create(2.0f);
    CCActionInterval* fadeOut = CCFadeOut::create(2.0f);
    CCSequence* fadeInAndOut = CCSequence::createWithTwoActions(fadeIn, fadeOut);
    logoTeamA->setOpacity(0);
    logoTeamA->runAction(fadeInAndOut);
    
    this->addChild(logoTeamA);
    
    this->schedule(schedule_selector(LogoScene::nextScene), 3.5f);
    
    return true;
}

void LogoScene::nextScene()
{
    CCScene* title = TitleScene::scene();
    CCTransitionFade* tran = CCTransitionFade::create(2.0f, title, ccc3(0, 0, 0));
    CCDirector::sharedDirector()->replaceScene(tran);
}