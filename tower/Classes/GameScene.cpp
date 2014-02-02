//
//  GameScene.cpp
//  nyanshuffle
//
//  Created by cocos2d-x on 2014/01/13.
//
//

#include "GameScene.h"
#include "picojson.h"
#include <fstream>

// For Character
#include "CharacterManager.h"

GameScene::GameScene() {
    _characterManager = NULL;
    _start = false;
}

GameScene::~GameScene() {
    if (_characterManager != NULL) {
        delete _characterManager;
        _characterManager = NULL;
    }
}

bool GameScene::init() {
    if (!CCLayer::init()) {
        return false;
    }
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    //マップファイルの読み込み
    this->mapList = this->readMapFile();
    
    parentNode1 = CCNode::create();
    parentNode1->setContentSize(ccp(winSize.width, winSize.height * 2));
    parentNode1->setPosition(ccp(winSize.width * 0.5, winSize.height));
    this->addChild(parentNode1);
    
    parentNode2 = CCNode::create();
    parentNode2->setContentSize(ccp(winSize.width, winSize.height * 2));
    parentNode2->setPosition(ccp(winSize.width * 0.5, winSize.height * 3));

    this->addChild(parentNode2);
    
    //set background
    bg1 = CCSprite::create(backGroundImgName);
    bg1->setPosition(CCPointZero);
    parentNode1->addChild(bg1);
    
    //set second background
    bg2 = CCSprite::create(backGroundImgName);
    bg2->setPosition(CCPointZero);
    parentNode2->addChild(bg2);
    
    CCLOG("1 anchor point x %f", bg1->getAnchorPoint().x);
    CCLOG("1 anchor point y %f", bg1->getAnchorPoint().y);
    //初期の板の位置を指定
    this->initMap();
    
    //test data
    testGameTime = 0;
    
    this->schedule(schedule_selector(GameScene::testScheduleMethod));
    
    // =====================================================================
    // For Character
    // 背景画像の両端の無効領域を算出
    const float backgroundOffsetPixcel = bg1->getTexture()->getPixelsWide() * 29.0f / 384.0f;
    
    initCharacter(backgroundOffsetPixcel);
    
    // タッチを有効にする
    this->setTouchEnabled(true);
    
    return true;
}

CCScene* GameScene::scene() {
    CCScene* scene = CCScene::create();
    GameScene* layer = GameScene::create();
    scene->addChild(layer);
    
    return scene;
}

std::vector<std::map<std::string,int> > GameScene::readMapFile() {
    const std::string MapFile = "boardMap.json";
    std::string MapFileFull = CCFileUtils::sharedFileUtils()->fullPathForFilename(MapFile.c_str());
    CCLOG("Reading %s",MapFileFull.c_str());
    std::ifstream fp(MapFileFull.c_str(),std::ifstream::binary);
    std::vector<std::map<std::string,int> > resultList;
    if (fp) {
        fp.seekg (0, fp.end);
        int length = fp.tellg();
        fp.seekg (0, fp.beg);
        
        char * buffer = new char [length];
        fp.read(buffer, length);
        
        if (fp) {
            CCLOG("read file success!!");
        } else {
            CCLOG("error: only %d could be read",fp.gcount());
        }
        
        fp.close();
        picojson::value v;
        std::string error;
        picojson::parse(v,buffer,buffer + strlen(buffer), &error);
        if ( !error.empty()) {
            CCLOG("Json perse fail");
        } else {
            CCLOG("Json perse sccuess");
            
            picojson::object &root = v.get<picojson::object>();
            picojson::array &results = root["results"].get<picojson::array>();
            picojson::array::iterator it;
            for (it = results.begin(); it != results.end(); it++) {
                picojson::object& num = it->get<picojson::object>();
                int x = num["x"].get<double>();
                int y = num["y"].get<double>();
                std::map<std::string,int> resultMap;
                resultMap.insert(std::pair<std::string, int>("x",x));
                resultMap.insert(std::pair<std::string, int>("y",y));
                resultList.push_back(resultMap);
                CCLOG("X: %d Y:%d",x,y);
            }
        }

        delete[] buffer;

    } else {
        CCLOG("file not found!!");
    }
    
    return resultList;
}

void GameScene::initMap() {
    if (this->mapList.empty()) {
        CCLOG("No MapList Found");
        return;
    }
   
    generateMap(parentNode1);
    generateMap(parentNode2);
}

void GameScene::moveMap(float amount) {
    
    CCMoveBy* moveA = CCMoveBy::create(3, ccp(0, -amount));
    CCMoveBy* moveB = CCMoveBy::create(3, ccp(0, -amount));
    parentNode1->runAction(moveA);
    parentNode2->runAction(moveB);
 
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    //背景が完全に隠れた場合は、上に動かして新しい板を作る
    if (parentNode1->getPositionY() <= -winSize.height || parentNode2->getPositionY() <= -winSize.height) {
        CCLOG("new background created.");
        
        if (parentNode1->getPositionY() <= -winSize.height) {
            parentNode1->setVisible(false);
            parentNode1->setPosition(ccp(winSize.width * 0.5, winSize.height * 3));
            generateMap(parentNode1);
        } else if (parentNode2->getPositionY() <= -winSize.height) {
            parentNode2->setVisible(false);
            parentNode2->setPosition(ccp(winSize.width * 0.5, winSize.height * 3));
            generateMap(parentNode2);
        }
    }
}

void GameScene::testScheduleMethod(float fDelta) {

    testGameTime += fDelta;
    int calcTime = (int) testGameTime;
    bool movedFlg = false;
    if (calcTime == lastValue) {
        movedFlg = true;
    }
    if (calcTime % 5 == 0 && !movedFlg) {
        lastValue = calcTime;
        movedFlg = true;
        this->moveMap(CCDirector::sharedDirector()->getWinSize().height / 2);
    }
}

void GameScene::generateMap(CCNode* parentNode) {
    //cleanup
    parentNode->removeAllChildren();
    CCSprite* bg = CCSprite::create(backGroundImgName);
    bg->setPosition(CCPointZero);
    parentNode->addChild(bg);
    
    CCPoint parentAP = parentNode->getAnchorPoint();
    CCSize parentCS = parentNode->getContentSize();
    
    float prevY = parentCS.height * -0.5;
    srand(time(NULL));
    
    while (prevY < parentCS.height) {
        CCSprite* board = CCSprite::create(boardImgName);
        float mPx = 15.0f;
        float mPy = 7.0f;
        
        std::map<std::string,int> location = this->mapList.at(rand() % this->mapList.size());
        prevY += (float)location.at("y") * mPy;
        if (prevY > parentCS.height) {
            break;
        }
        float xPos = (float) location.at("x") * mPx;
        board->setPosition(ccp(parentCS.width * parentAP.x + xPos, parentCS.height * parentAP.y + prevY));
        parentNode->addChild(board);
        
    }
    parentNode->setVisible(true);
}

// =====================================================================
// For Character
void GameScene::update(float dt) {
    if (_characterManager != NULL) _characterManager->update(dt);
}

void GameScene::startGame() {
    if (_characterManager != NULL) _characterManager->setVisible(true);
    
    scheduleUpdate();
}

bool GameScene::initCharacter(const float backgroundOffsetPixcel) {
    if (_characterManager == NULL) _characterManager = new CharacterManager();
    
    if (_characterManager == NULL) return false;
    
    //_characterManager->createCCSpriteBatchNode(TEST_CHARA_PNG);
    
    CharacterBase* enemy0 = new CharacterBase();
    
    if (enemy0 == NULL) return false;
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    /*
     CCSpriteBatchNode* spr = _characterManager->getCCSpriteBatchNode();
     CCTexture2D*       tex = spr->getTexture();
     
     int pixW     = tex->getPixelsWide();
     int pixH     = tex->getPixelsHigh();
     int pixHalfW = (int)((float)pixW * 0.5f);
     int pixHalfH = (int)((float)pixH * 0.5f);
     
     CCRect enemyR = CCRectMake(pixHalfW, pixHalfH, pixW,     pixH);
     CCRect enemyL = CCRectMake(0,        0,        pixHalfW, pixHalfH);
     
     enemy0->createCCSprite(tex, enemyR, enemyL, backgroundOffsetPixcel);
     */
    enemy0->createCCSprite(TEST_CHARA_PNG, TEST_CHARA_PNG, backgroundOffsetPixcel);
    enemy0->setPosition(visibleSize.width * 0.35f, visibleSize.height * 0.75f);
    
    _characterManager->addCharacter(enemy0);
    
    //this->addChild(spr);
    _characterManager->setSprite(this);
    
    return true;
}

bool GameScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
    if (_start) {
        if (_characterManager != NULL) _characterManager->setJump();
    }
    else {
        _start = true;
        startGame();
    }
    
    return true;
}

void GameScene::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -127, true);
}
