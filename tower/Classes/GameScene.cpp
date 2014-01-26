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

bool GameScene::init() {
    if (!CCLayer::init()) {
        return false;
    }
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    //マップファイルの読み込み
    this->mapList = this->readMapFile();
    
    CCNode* bgNode = CCNode::create();
    bgNode->setAnchorPoint(CCPoint(0.5f, 0.5f));
    bgNode->setContentSize(ccp(winSize.width, winSize.height));
    bgNode->setPosition(ccp(winSize.width * 0.5, winSize.height * 0.5));
    this->addChild(bgNode);
    
    //set background
    CCSprite* pBG1 = CCSprite::create(backGroundImgName);
    pBG1->setPosition(ccp(winSize.width * 0.5, winSize.height * 0.5));
    bgNode->addChild(pBG1);
    
    
    //set second background
    CCSprite* pBG2 = CCSprite::create(backGroundImgName);
    pBG2->setPosition(ccp(1 + winSize.width * 0.5, 1 + winSize.height * 0.5));
    bgNode->addChild(pBG2);
    
    
    
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

void GameScene::initMap(CCNode* parentNode) {
    if (this->mapList.empty()) {
        CCLOG("No MapList Found");
        return;
    }
   
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    float prevY = 0;
    float mP = 1.0f;
    srand(time(NULL));
    while (prevY < winSize.height * 2) {
        CCSprite* board = CCSprite::create(boardImgName);
        if (prevY == 0) {
            prevY = winSize.height * 0.1;
            board->setPosition(ccp(winSize.width * 0.5, winSize.height * 0.1));
            parentNode->addChild(board);
        } else {
            std::map<std::string,int> location = this->mapList.at(rand() % this->mapList.size());
            prevY += (float)location.at("y");
            float xPos = (float) location.at("x");
            board->setPosition(ccp(winSize * 0.5f + xPos * mP, prevY));
        }
        this->mapSpriteList.push_back(board);
        
    }
    
    
}