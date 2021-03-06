//
//  MCRoleBaseInfo.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-17.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "AppMacros.h"
#include "MCRoleBaseInfo.h"
#include "MCActionBar.h"
#include "MCEffectiveItem.h"
#include "MCSimpleAudioEngine.h"

static const char *kMCRoleBaseInfoFaceBoxFilepath = "UI/bi_face_box.png";
static const char *kMCRoleBaseInfoFaceBoxSelectedFilepath = "UI/bi_face_box_selected.png";
static const char *kMCRoleBaseInfoValueBoxFilepath = "UI/rbi_value_box.png";

static const char *kMCPotionEffecFilepath = "voices/potion.wav";

static const float kMCFontSize = 18;

#pragma mark *** MCRoleBaseInfo ***

bool
MCRoleBaseInfo::init(MCRole *aRole)
{
    if (CCLayer::init()) {
        CCPoint anchorPoint = ccp(0, 1);
        CCPoint labelAnchorPoint = ccp(0, 0.05);
        CCSize labelSize;
        CCPoint labelPosition;
        CCLabelTTF *separatorLabel;
        float y;
        float contentScaleFactor = CC_CONTENT_SCALE_FACTOR();
        
        CCSize faceBoxSize = CCSizeMake(72 / contentScaleFactor,
                                        72 / contentScaleFactor);
        CCRect faceBoxSourceRect = CCRectMake(0, 0, 34, 34);
        CCRect faceBoxInseRect = CCRectMake(4, 4, 30, 30);
        
        faceBox_ = CCScale9Sprite::create(kMCRoleBaseInfoFaceBoxFilepath,
                                          faceBoxSourceRect,
                                          faceBoxInseRect);
        addChild(faceBox_);
        faceBox_->setContentSize(faceBoxSize);
        faceBox_->setAnchorPoint(anchorPoint);
        faceBox_->setPosition(ccp(0, 0));
        
        face_ = CCSprite::create(aRole->getFace()->getCString());
        addChild(face_);
        face_->setAnchorPoint(anchorPoint);
        face_->setScale(64 / contentScaleFactor / face_->getContentSize().height);
        face_->setPosition(ccp(4 / contentScaleFactor, -4 / contentScaleFactor));
        
        faceBoxSelected_ = CCScale9Sprite::create(kMCRoleBaseInfoFaceBoxSelectedFilepath,
                                                  faceBoxSourceRect,
                                                  faceBoxInseRect);
        addChild(faceBoxSelected_);
        faceBoxSelected_->setContentSize(faceBoxSize);
        faceBoxSelected_->setAnchorPoint(anchorPoint);
        faceBoxSelected_->setPosition(ccp(0, 0));
        faceBoxSelected_->setVisible(false);
        
        CCSprite *valueBox = CCSprite::create(kMCRoleBaseInfoValueBoxFilepath);
        addChild(valueBox);
        valueBox->setOpacity(196);
        valueBox->setAnchorPoint(anchorPoint);
        valueBox->setPosition(ccp(faceBox_->getPositionX() + faceBoxSize.width, 0));
        
        /* PP */
        CCString *ccstring = CCString::createWithFormat("%-03.0f", aRole->getPP());
        ppLabel_ = CCLabelTTF::create(ccstring->getCString(), "Marker Felt", kMCFontSize);
        addChild(ppLabel_);
        labelSize = ppLabel_->getContentSize();
        ppLabel_->setColor(ccc3(240, 240, 240));
        ppLabel_->setAnchorPoint(labelAnchorPoint);
        y = labelSize.height / 2 - faceBoxSelected_->getContentSize().height;
        ppLabel_->setPosition(ccp(96 / contentScaleFactor, y));
        ccstring->retain();
        
        separatorLabel = CCLabelTTF::create(" / ", "Marker Felt", kMCFontSize);
        addChild(separatorLabel);
        separatorLabel->setColor(ccc3(240, 240, 240));
        separatorLabel->setAnchorPoint(labelAnchorPoint);
        separatorLabel->setPosition(ccp(96 / contentScaleFactor + labelSize.width, y));
        
        labelPosition = separatorLabel->getPosition();
        ccstring = CCString::createWithFormat("%03.0f", aRole->getMaxPP());
        maxPPLabel_ = CCLabelTTF::create(ccstring->getCString(), "Marker Felt", kMCFontSize);
        addChild(maxPPLabel_);
        maxPPLabel_->setColor(ccc3(240, 240, 240));
        maxPPLabel_->setAnchorPoint(labelAnchorPoint);
        maxPPLabel_->setPosition(ccp(labelPosition.x + separatorLabel->getContentSize().width, y));
        ccstring->retain();
        
        /* HP */
        ccstring = CCString::createWithFormat("%-03hu", aRole->getHP());
        hpLabel_ = CCLabelTTF::create(ccstring->getCString(), "Marker Felt", kMCFontSize);
        addChild(hpLabel_);
        labelSize = hpLabel_->getContentSize();
        hpLabel_->setColor(ccc3(51, 153, 51));
        hpLabel_->setAnchorPoint(labelAnchorPoint);
        y += labelSize.height;
        hpLabel_->setPosition(ccp(96 / contentScaleFactor, y));
        ccstring->retain();
        
        separatorLabel = CCLabelTTF::create(" / ", "Marker Felt", kMCFontSize);
        addChild(separatorLabel);
        separatorLabel->setColor(ccc3(51, 153, 51));
        separatorLabel->setAnchorPoint(labelAnchorPoint);
        separatorLabel->setPosition(ccp(96 / contentScaleFactor + labelSize.width, y));
        
        labelPosition = separatorLabel->getPosition();
        ccstring = CCString::createWithFormat("%03hu", aRole->getMaxHP());
        maxHPLabel_ = CCLabelTTF::create(ccstring->getCString(), "Marker Felt", kMCFontSize);
        addChild(maxHPLabel_);
        maxHPLabel_->setColor(ccc3(51, 153, 51));
        maxHPLabel_->setAnchorPoint(labelAnchorPoint);
        maxHPLabel_->setPosition(ccp(labelPosition.x + separatorLabel->getContentSize().width, y));
        ccstring->retain();
        
        role_ = aRole;
        aRole->retain();
        
        width_ = faceBox_->getContentSize().width + valueBox->getContentSize().width;
        height_ = faceBox_->getContentSize().height;
        touched_ = false;
        
        return true;
    }
    
    return false;
}

MCRoleBaseInfo *
MCRoleBaseInfo::create(MCRole *aRole)
{
    MCRoleBaseInfo *info = new MCRoleBaseInfo;
    
    if (info && info->init(aRole)) {
        info->autorelease();
    } else {
        CC_SAFE_DELETE(info);
        info = NULL;
    }
    
    return info;
}

void
MCRoleBaseInfo::updateInfo(float dt)
{
    CCString *ccstring;
    MCRole *role = role_;
    
    ccstring = CCString::createWithFormat("%-03hu", role->getHP());
    hpLabel_->setString(ccstring->getCString());
    ccstring = CCString::createWithFormat("%-03.0f", role->getPP());
    ppLabel_->setString(ccstring->getCString());
}

void
MCRoleBaseInfo::onEnter()
{
    CCLayer::onEnter();
    schedule(schedule_selector(MCRoleBaseInfo::updateInfo));
}

void
MCRoleBaseInfo::onExit()
{
    unschedule(schedule_selector(MCRoleBaseInfo::updateInfo));
    CCLayer::onExit();
}

/**
 * 使用成功返回true，否则false
 */
bool
MCRoleBaseInfo::useActionBarItem(MCActionBarItem *anActionBarItem)
{
    MCBackpackItem *backpackItem = (MCBackpackItem *) anActionBarItem->getBackpackItem();
    if (backpackItem->use()) { /* 能够使用才执行，已经减去1 */
        MCEffectiveItem *item = (MCEffectiveItem *) backpackItem->item;
        
        role_->updateHP(item->hp);
        role_->updatePP(item->pp);
        MCSimpleAudioEngine::sharedSimpleAudioEngine()->playEffect(kMCPotionEffecFilepath);
        
        return true;
    }
    
    return false;
}

#pragma mark -
#pragma mark *** MCRoleBaseInfoGroup ***

MCRoleBaseInfoGroup::MCRoleBaseInfoGroup()
{
    team_ = MCTeam::sharedTeam();
    infoList_ = CCArray::create();
    infoList_->retain();
    
    /* hero失踪之谜！折中解决方案~ */
    team_->ensureHero();
    
    CCArray *roles = team_->getRoles();
    CCObject *obj;
    MCRole *role;
    CCARRAY_FOREACH(roles, obj) {
        role = (MCRole *) obj;
        MCRoleBaseInfo *info = MCRoleBaseInfo::create(role);
        addChild(info);
        infoList_->addObject(info);
    }
    width_ = 0;
    height_ = 0;
    align();
}

MCRoleBaseInfoGroup::~MCRoleBaseInfoGroup()
{
    CC_SAFE_RELEASE(infoList_);
}

MCRoleBaseInfo *
MCRoleBaseInfoGroup::roleBaseInfoForRole(MCRole *aRole)
{
    if (aRole == NULL) {
        return NULL;
    }
    
    CCObject *obj;
    MCRoleBaseInfo *info;
    CCARRAY_FOREACH(infoList_, obj) {
        info = dynamic_cast<MCRoleBaseInfo *>(obj);
        if (info->getRole() == aRole) {
            return info;
        }
    }
    
    return NULL;
}

void
MCRoleBaseInfoGroup::addRoleBaseInfo(MCRoleBaseInfo *anInfo)
{
    if (infoList_->count() < team_->getMaxSize()) {
        addChild(anInfo);
        infoList_->addObject(anInfo);
        team_->addRole(anInfo->getRole());
        align();
    }
}

void
MCRoleBaseInfoGroup::removeRoleBaseInfo(MCRoleBaseInfo *anInfo)
{
    team_->removeRole(anInfo->getRole());
    infoList_->removeObject(anInfo);
    removeChild(anInfo);
    align();
}

mc_size_t
MCRoleBaseInfoGroup::size()
{
    return infoList_->count();
}

void
MCRoleBaseInfoGroup::align()
{
    CCSize winSize = CCDirectorGetWindowsSize();
    CCObject *obj;
    MCRoleBaseInfo *info;
    float y = winSize.height;
    float contentScaleFactor = CC_CONTENT_SCALE_FACTOR();
    CCPoint anchorPoint = ccp(0, 1);
    CCARRAY_FOREACH(infoList_, obj) {
        info = (MCRoleBaseInfo *) obj;
        info->setAnchorPoint(anchorPoint);
        info->setPosition(ccp(0, y));
        width_ = info->getWidth();
        height_ += info->getHeight();
        y -= 73.f / contentScaleFactor;
    }
}
