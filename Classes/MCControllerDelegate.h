//
//  MCControllerDelegate.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-23.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCControllerDelegate__
#define __Military_Confrontation__MCControllerDelegate__

#include <cocos2d.h>
#include "MCType.h"

USING_NS_CC;

class MCRole;
class MCTeam;
class MCItem;

/* 控制器代理 */
class MCControllerDelegate {
public:
    /* joypad */
    virtual void controllerDidMove(MCControllerDelegate *aSender, const CCPoint &delta) {}
    
    /* battle control */
    /**
     * 选择了人物。
     */
    virtual void controllerDidSelectRole(MCControllerDelegate *aSender, MCRole *aSelectedRole) {}
    
    /**
     * 取消了选择的人物。
     */
    virtual void controllerDidUnselectRole(MCControllerDelegate *aSender, MCRole *aSelectedRole) {}
    
    /**
     * 选择全部，若已经全部选择，则全部取消选择。
     */
    virtual void controllerDidSelectAll(MCControllerDelegate *aSender, MCTeam *aTeam) {}
    
    /**
     * 进入多选模式
     */
    virtual void controllerDidEnterMultiSelectionMode(MCControllerDelegate *aSender) {}
    
    /**
     * 退出多选模式
     */
    virtual void controllerDidExitMultiSelectionMode(MCControllerDelegate *aSender) {}
    
    /**
     * 在选择了人物的情况下，指定移动到某个位置(在地图上的)
     */
    virtual void controllerDidPointTo(MCControllerDelegate *aSender, const CCPoint &locationAtMap) {}
    
    /**
     * 是否允许拖动anItem。按下图标的时候执行。
     */
    virtual bool controllerShouldDragItem(MCControllerDelegate *aSender, MCItem *anItem) { return false; }
    
    /**
     * 将要开始拖动anItem。按下图标后，首次移动anItem的时候执行。
     */
    virtual void controllerWillDragItem(MCControllerDelegate *aSender, MCItem *anItem) {}
    
    /**
     * 拖动完anItem，拖动到了人物aRole上，若aRole等于NULL，则表示没有拖动到任何人物上。放开anItem的时候执行。
     */
    virtual void controllerDidFinishDragItem(MCControllerDelegate *aSender, MCItem *anItem, MCRole *aRole) {}
    
    /**
     * 选择了anItem。按下和放开手指都在anItem的范围内时执行。
     */
    virtual void controllerDidSelectItem(MCControllerDelegate *aSender, MCItem *anItem) {}
    
    virtual void controllerDid(MCControllerDelegate *aSender) {}
};

#endif /* defined(__Military_Confrontation__MCControllerDelegate__) */
