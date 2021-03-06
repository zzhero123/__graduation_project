//
//  MCActionBar.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-26.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "AppMacros.h"
#include "MCActionBar.h"
#include "MCTaskManager.h"
#include "MCTaskContext.h"

/* extern */
const GLubyte kMCInvalidActionBarItemOpacity  = 140;
const GLubyte kMCDraggingActionBarItemOpacity = 180;
const GLubyte kMCNormalActionBarItemOpacity   = 255;

static const char *kMCActionBarItemOpenedFilepath = "UI/abi_opened.png";
static const char *kMCActionBarItemOpenedSelectedFilepath = "UI/abi_opened_selected.png";
static const char *kMCActionBarItemClosedFilepath = "UI/abi_closed.png";
static const char *kMCActionBarItemClosedSelectedFilepath = "UI/abi_closed_selected.png";

static const float kMCActionDuration = 0.2f;

#pragma mark *** MCActionBarItem ***

bool
MCActionBarItem::init(MCBackpackItem *aBackpackItem)
{
    if (CCSprite::initWithFile(aBackpackItem->item->getIcon()->getCString())) {
        CCPoint anchorPoint = ccp(0.5f, 0.0f);
        
        count_ = CCLabelTTF::create(CCString::createWithFormat("%hi", aBackpackItem->count)->getCString(),
                                    "",
                                    24);
        addChild(count_);
        count_->setAnchorPoint(anchorPoint);
        count_->setColor(ccc3(32, 32, 32));
        count_->setPosition(ccp(this->getContentSize().width / 2, 2));
        
        if (aBackpackItem->count == 0) {
            setOpacity(kMCInvalidActionBarItemOpacity);
        }
        
        backpackItem_ = aBackpackItem;
        
        return true;
    }
    
    return false;
}

MCActionBarItem *
MCActionBarItem::create(MCBackpackItem *aBackpackItem)
{
    MCActionBarItem *item = new MCActionBarItem;
    
    if (item && item->init(aBackpackItem)) {
        item->autorelease();
    } else {
        CC_SAFE_DELETE(item);
        item = NULL;
    }
    
    return item;
}

void 
MCActionBarItem::resetPosition()
{
    setPosition(itemPosition_);
}

CCPoint
MCActionBarItem::getItemPosition()
{
    return itemPosition_;
}

void
MCActionBarItem::setItemPosition(CCPoint var)
{
    setPosition(var);
    itemPosition_ = var;
}

void
MCActionBarItem::updateCount()
{
    count_->setString(CCString::createWithFormat("%hu", backpackItem_->count)->getCString());
}

#pragma mark -
#pragma mark *** MCActionBar ***

MCActionBar::~MCActionBar()
{
    CC_SAFE_RELEASE(trapWide_);
    CC_SAFE_RELEASE(trapDamage_);
    CC_SAFE_RELEASE(healthPotion_);
    CC_SAFE_RELEASE(physicalPotion_);
}

bool
MCActionBar::init()
{
    if (CCLayer::init()) {
        MCTaskContext *taskContext = MCTaskManager::sharedTaskManager()->getCurrentTask()->getTaskContext();
        CCPoint anchorPoint = ccp(0.5f, 0.0f);
        
        trapWide_ = MCActionBarItem::create(taskContext->getTrapWide());
        addChild(trapWide_);
        trapWide_->setAnchorPoint(anchorPoint);
        trapDamage_ = MCActionBarItem::create(taskContext->getTrapDamage());
        addChild(trapDamage_);
        trapDamage_->setAnchorPoint(anchorPoint);
        healthPotion_ = MCActionBarItem::create(taskContext->getHealthPotion());
        addChild(healthPotion_);
        healthPotion_->setAnchorPoint(anchorPoint);
        physicalPotion_ = MCActionBarItem::create(taskContext->getPhysicalPotion());
        addChild(physicalPotion_);
        physicalPotion_->setAnchorPoint(anchorPoint);
        
        CCMenuItemImage *opened = CCMenuItemImage::create(kMCActionBarItemOpenedFilepath,
                                                          kMCActionBarItemOpenedSelectedFilepath);
        CCMenuItemImage *closed = CCMenuItemImage::create(kMCActionBarItemClosedFilepath,
                                                          kMCActionBarItemClosedSelectedFilepath);
        CCMenuItemToggle *toggleButton = CCMenuItemToggle::createWithTarget(this,
                                                                            menu_selector(MCActionBar::toggle),
                                                                            opened,
                                                                            closed,
                                                                            NULL);
        toggleButton_ = CCMenu::createWithItem(toggleButton);
        addChild(toggleButton_);
        toggleButton->setAnchorPoint(anchorPoint);
        
        align();
        
        return true;
    }
    
    return false;
}

void
MCActionBar::toggle()
{
    CCSize size = trapWide_->getContentSize();
    CCPoint offset = ccp(0, size.height * 2);
    if (trapWide_->getPositionY() >= 0) { /* 将要隐藏 */
        offset.y = -offset.y;
    }
    trapWide_->runAction(CCMoveBy::create(kMCActionDuration, offset));
    trapDamage_->runAction(CCMoveBy::create(kMCActionDuration, offset));
    healthPotion_->runAction(CCMoveBy::create(kMCActionDuration, offset));
    physicalPotion_->runAction(CCMoveBy::create(kMCActionDuration, offset));
}

MCActionBarItem *
MCActionBar::itemForTouch(CCTouch *pTouch)
{
    CCPoint touchLocation = pTouch->getLocation();

    /* trapWide_ */
    CCPoint local = trapWide_->convertToNodeSpace(touchLocation);
    CCSize s = trapWide_->getContentSize();
    CCRect r = CCRectMake(0, 0, s.width, s.height);
    if (r.containsPoint(local)) {
        return trapWide_;
    }
    /* trapDamage_ */
    local = trapDamage_->convertToNodeSpace(touchLocation);
    s = trapDamage_->getContentSize();
    r = CCRectMake(0, 0, s.width, s.height);
    if (r.containsPoint(local)) {
        return trapDamage_;
    }
    /* healthPotion_ */
    local = healthPotion_->convertToNodeSpace(touchLocation);
    s = healthPotion_->getContentSize();
    r = CCRectMake(0, 0, s.width, s.height);
    if (r.containsPoint(local)) {
        return healthPotion_;
    }
    /* physicalPotion_ */
    local = physicalPotion_->convertToNodeSpace(touchLocation);
    s = physicalPotion_->getContentSize();
    r = CCRectMake(0, 0, s.width, s.height);
    if (r.containsPoint(local)) {
        return physicalPotion_;
    }
    
    return NULL;
}

void
MCActionBar::align()
{
    CCSize winSize = CCDirectorGetWindowsSize();
    CCSize itemSize = trapWide_->getContentSize();
    CCPoint centerPoint = ccp(winSize.width / 2, 0);
    CCPoint itemPosition = ccp(centerPoint.x - itemSize.width * 1.5, centerPoint.y);
    
    trapWide_->setItemPosition(itemPosition);
    itemPosition = ccp(centerPoint.x - itemSize.width * 0.5, centerPoint.y);
    trapDamage_->setItemPosition(itemPosition);
    itemPosition = ccp(centerPoint.x + itemSize.width * 0.5, centerPoint.y);
    healthPotion_->setItemPosition(itemPosition);
    itemPosition = ccp(centerPoint.x + itemSize.width * 1.5, centerPoint.y);
    physicalPotion_->setItemPosition(itemPosition);
    itemPosition = ccp(centerPoint.x + itemSize.width * 2.5, centerPoint.y - 13);
    toggleButton_->setPosition(itemPosition);
}
