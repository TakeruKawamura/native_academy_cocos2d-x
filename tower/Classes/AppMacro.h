//
//  AppMacro.h
//  tower
//
//  Created by cocos2d-x on 2014/01/26.
//
//

#ifndef __APPMACROS_H__
#define __APPMACROS_H__

#include "cocos2d.h"

typedef struct tagResource {
    cocos2d::CCSize size;
    char directory[100];
} Resource;

static Resource smallResource = {cocos2d::CCSizeMake(192,256),"resources-small"};
static Resource largeResource = {cocos2d::CCSizeMake(384,512),"resources-large"};
static Resource xlargeResource = {cocos2d::CCSizeMake(768, 1024),"resources-xlarge"};

static cocos2d::CCSize designResolutionSize = cocos2d::CCSizeMake(768,1024);

#define LEVEL_FONT_SIZE (cocos2d::CCEGLView::sharedOpenGLView()->getDesignResolutionSize().width / mediumResource.size.width * 48)

#endif //__APPMACROS_H__
