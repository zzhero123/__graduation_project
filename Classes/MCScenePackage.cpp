//
//  MCScenePackage.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-24.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "MCScenePackage.h"
#include "MCRoleManager.h"
#include "MCEntrance.h"
#include "MCFlagManager.h"

MCScenePackage::~MCScenePackage()
{
    CC_SAFE_RELEASE(triggerFilepath_);
    
    CC_SAFE_RELEASE(objects_);
    CC_SAFE_RELEASE(scenes_);
    
    CC_SAFE_RELEASE(tmxTiledMapPath_);
    CC_SAFE_RELEASE(backgroundMusicPath_);
}

bool
MCScenePackage::init()
{
    do {
        objects_ = CCArray::create();
        objects_->retain();
        
        scenes_ = CCDictionary::create();
        CC_BREAK_IF(! scenes_);
        scenes_->retain();
        
        triggerFilepath_ = NULL;
        tmxTiledMapPath_ = NULL;
        backgroundMusicPath_ = NULL;
        
        return true;
    } while (0);
    
    return false;
}

MCScenePackage *
MCScenePackage::create(const char *aPackagePath)
{
    MCScenePackage *package = new MCScenePackage;
    
    if (package && package->init()) {
        package->loadFromFile(aPackagePath);
        package->autorelease();
        return package;
    } else {
        CC_SAFE_DELETE(package);
    }
    
    return NULL;
}

//MCRole *
//MCScenePackage::objectForObjectId(mc_object_id_t anObjectId)
//{
//    return (MCRole *) objects_->objectForKey(MCObjectIdToDickKey(anObjectId));
//}

void
MCScenePackage::loadFromFile(const char *aPackagePath)
{
    JsonBox::Value in;
    JsonBox::Object root;
    JsonBox::Object object;
    const char *c_str_o_id;
    
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCString* pstrFileContent = CCString::createWithContentsOfFile(aPackagePath);
    if (pstrFileContent) {
        in.loadFromString(pstrFileContent->getCString());
    }
#else
    in.loadFromFile(CCFileUtils::sharedFileUtils()->fullPathForFilename(aPackagePath).c_str());
#endif

    root = in.getObject();
    
    /* ID String */
    c_str_o_id = root["id"].getString().c_str();
    mc_object_id_t o_id = {
        c_str_o_id[0],
        c_str_o_id[1],
        c_str_o_id[2],
        c_str_o_id[3]
    };
    setID(o_id);
    
    /* type int */
    scenePackageType_ = root["scene-type"].getInt();
    
    /* internal int */
    isInternalScene_ = root["internal"].getInt() == 1 ? true : false;
    
    /* name String */
    name_ = CCString::create(root["name"].getString());
    name_->retain();
    
    /* trigger String */
    if (root["trigger"].isString()) {
        triggerFilepath_ = CCString::create(root["trigger"].getString());
        triggerFilepath_->retain();
    }
    
    /* description String */
    description_ = CCString::create(root["description"].getString());
    description_->retain();
    
    /* objects Array */
    loadObjects(root);
    
    /* background Object */
    object = root["background"].getObject();
    /* background["tmx"] String */
    tmxTiledMapPath_ = CCString::create(object["tmx"].getString());
    tmxTiledMapPath_->retain();
    /* background["sound"] String */
    backgroundMusicPath_ = CCString::create(object["sound"].getString());
    backgroundMusicPath_->retain();
    
    /* scenes Object */
    loadScenes(root);
}

void
MCScenePackage::loadObjects(JsonBox::Object &aRoot)
{
    JsonBox::Array objects = aRoot["objects"].getArray();
    JsonBox::Value v;
    JsonBox::Object roles;
    JsonBox::Array flags;
    JsonBox::Array::iterator flagsIterator;
    JsonBox::Array::iterator objectsIterator;
    JsonBox::Object roleObject;
    const char *c_str_o_id;
    MCRoleManager *roleManager = MCRoleManager::sharedRoleManager();

    /* load objects */
    for (objectsIterator = objects.begin(); objectsIterator != objects.end(); ++objectsIterator) {
        roleObject = objectsIterator->getObject();
        c_str_o_id = roleObject["id"].getString().c_str();
        mc_object_id_t o_id = {
            c_str_o_id[0],
            c_str_o_id[1],
            c_str_o_id[2],
            c_str_o_id[3]
        };
        MCRole *role = roleManager->roleForObjectId(o_id);
        if (role) {
            /* tags: #role #metadata #init-position */
            MCRoleEntityMetadata *metadata = role->getEntityMetadata();
            metadata->setPosition(ccp(roleObject["x"].getInt(), roleObject["y"].getInt()));
            flags = roleObject["flags"].getArray();
            CCArray *flagsArray = metadata->getFlags();
            for (flagsIterator = flags.begin();
                 flagsIterator != flags.end();
                 ++flagsIterator) {
                const char *c_str_flag_id = flagsIterator->getString().c_str();
                mc_object_id_t flag_id = {
                    c_str_flag_id[0],
                    c_str_flag_id[1],
                    c_str_flag_id[2],
                    c_str_flag_id[3]
                };
                flagsArray->addObject(MCFlagManager::sharedFlagManager()->flagForObjectId(flag_id));
            }
            objects_->addObject(role);
        }
    }
}

/**
 * 从数据包可以读取到除了入口坐标之外的内容。
 * 所以在读取数据包的时候读取除了坐标之外的内容，坐标在载入TMX地图的时候附上。
 */
void
MCScenePackage::loadScenes(JsonBox::Object &aRoot)
{
    JsonBox::Object scenes = aRoot["scenes"].getObject();
    JsonBox::Object::iterator objectIterator;
    MCEntrance *entrance;
    const char *c_str_o_id;
    CCString *ccstring;
    
    for (objectIterator = scenes.begin(); objectIterator != scenes.end(); ++objectIterator) {
        entrance = new MCEntrance;
        entrance->autorelease();
        ccstring = CCString::create(objectIterator->first.c_str());
        entrance->setName(ccstring);
        ccstring->retain();
        JsonBox::Object destination = objectIterator->second.getObject();
        /* destination["id"] String */
        c_str_o_id = destination["id"].getString().c_str();
        mc_object_id_t o_id = {
            c_str_o_id[0],
            c_str_o_id[1],
            c_str_o_id[2],
            c_str_o_id[3]
        };
        ccstring = CCString::create(destination["destination"].getString());
        entrance->setDestination(ccstring);
        ccstring->retain();
        entrance->setID(o_id);
        scenes_->setObject(entrance, entrance->getName()->getCString());
    }
}
