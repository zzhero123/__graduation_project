//
//  MCEnemy.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-27.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCEnemy.h"
#include "MCEnemyAI.h"

bool
MCEnemy::init(MCRoleRace aRoleRace)
{
    roleType_ = MCRole::MCEnemy;
    setRoleRace(aRoleRace);
    face_ = NULL;
    defaultDialogue_ = NULL;
    
    return true;
}

MCEnemy *
MCEnemy::create(mc_object_id_t anObjectId)
{
    MCEnemy *enemy = new MCEnemy;
    
    if (enemy && enemy->init(MCTerrestrial)) {
        enemy->setID(anObjectId);
        enemy->autorelease();
        /* 加载enemy数据 */
        /* 检测怪物种族 */
    } else {
        CC_SAFE_DELETE(enemy);
        enemy = NULL;
    }
    
    return enemy;
}

MCRoleEntity *
MCEnemy::getEntity()
{
    MCRoleEntity *roleEntity = MCRole::getEntity();
    
    /* AI */
    if (ai_ == NULL) {
        ai_ = MCEnemyAI::create();
        ai_->retain();
        ai_->bind(this);
    }
    
    return roleEntity;
}

CCObject *
MCEnemy::copy()
{
    MCEnemy *enemy = new MCEnemy;
    
    enemy->id_ = id_;
    enemy->tag_ = tag_;
    enemy->name_ = CCString::create(name_->getCString()); /* 会被释放掉，所以要copy一个 */
    enemy->name_->retain();
    enemy->description_ = CCString::create(description_->getCString()); /* 会被释放掉，所以要copy一个 */
    enemy->description_->retain();
    
    enemy->roleType_ = roleType_;
    enemy->roleRace_ = roleRace_;
    enemy->hp_ = hp_;
    enemy->pp_ = pp_;
    enemy->maxHP_ = maxHP_;
    enemy->maxPP_ = maxPP_;
    enemy->exhaustion_ = exhaustion_;
    enemy->tired_ = tired_;
    enemy->dexterity_ = dexterity_;
    enemy->roleState_ = roleState_;
    enemy->face_ = NULL;
    enemy->spriteSheet_ = CCString::create(spriteSheet_->getCString()); /* 会被释放掉，所以要copy一个 */
    enemy->spriteSheet_->retain();
    enemy->defaultDialogue_ = NULL;
    enemy->ai_ = ai_;
    enemy->trigger_ = trigger_;
    
    return enemy;
}
