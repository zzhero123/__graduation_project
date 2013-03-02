//
//  MCRoleEntity.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-2-25.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCRoleEntity__
#define __Military_Confrontation__MCRoleEntity__

#include "MCObject.h"

/* 角色朝向 */
enum {
    MCFacingUnknown = 0,             /* 未知，尚未生成实体的时候 */
    MCFacingUp      = MCMakeEnum(0), /* 面朝上 */
    MCFacingDown    = MCMakeEnum(1), /* 面朝下 */
    MCFacingLeft    = MCMakeEnum(2), /* 面朝左 */
    MCFacingRight   = MCMakeEnum(3)  /* 面朝右 */
};
typedef mc_enum_t MCFacade;

class MCRole;

class MCRoleEntityMetadata : public CCSprite {
    friend class MCRole;
    friend class MCRoleEntity;
    
public:
    MCRoleEntityMetadata();
    ~MCRoleEntityMetadata();
    CCString *spriteSheetPath_;
    CCRect frameV_;
    CCRect frameH_;
    
    /* 显示相关 */
    CC_SYNTHESIZE_READONLY(CCSpriteBatchNode *, spriteSheet_, SpriteSheet);
    CC_SYNTHESIZE_READONLY(MCFacade, facade_, Facade);
    
    /* 移动动画 */
    CC_SYNTHESIZE_READONLY(CCAnimation *, animationGoUp_, AnimationGoUp);
    CC_SYNTHESIZE_READONLY(CCAnimation *, animationGoDown_, AnimationGoDown);
    CC_SYNTHESIZE_READONLY(CCAnimation *, animationGoLeft_, AnimationGoLeft);
    CC_SYNTHESIZE_READONLY(CCAnimation *, animationGoRight_, AnimationGoRight);
};

class MCRoleEntity : public CCSprite {
    friend class MCRole;
    
public:
    MCRoleEntity();
    ~MCRoleEntity();
    
    CCRect getBounds(); /* 碰撞域 */
    
    void walk(MCFacade aFacade);
    void walkTo(CCPoint &aDestinationPosition);
    void walkOnScreen(const CCPoint &aDestinationPosition, const CCPoint &offset);
    bool isWalking();
    void stopWalking();
    
    void walkEnded();
    
protected:
    void actionEnded(CCObject* anObject);
    void stopAllMoveToActions();
    
    CC_PROPERTY_READONLY(CCSpriteBatchNode *, spriteSheet_, SpriteSheet);
    CC_PROPERTY_READONLY(MCFacade, facade_, Facade);
    CC_PROPERTY_READONLY(MCRoleEntityMetadata *, metadata_, Metadata);
    
    CC_SYNTHESIZE_READONLY(MCRole *, role_, Prototype); /* 指向MCRole */
    
private:
    CCArray *moveToActions_;
};

#endif /* defined(__Military_Confrontation__MCRoleEntity__) */
