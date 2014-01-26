//
//  AppMacros.h
//  tower
//
//  Created by cocos2d-x on 14/01/26.
//
//

#ifndef __APPMACROS_H__
#define __APPMACROS_H__

#include "cocos2d.h"

typedef struct tagResource
{
    cocos2d::CCSize size;
    char directory[100];
} Resource;

static Resource smallResource = {cocos2d::CCSizeMake(256, 197), "resources-small"};
static Resource largeResource = {cocos2d::CCSizeMake(512, 384), "resources-large"};
static Resource xlargeResource = {cocos2d::CCSizeMake(1024, 768), "resources-xlarge"};

static cocos2d::CCSize designResolutionSize = cocos2d::CCSizeMake(1024, 768);

#endif /* __APPMACROS_H__ */