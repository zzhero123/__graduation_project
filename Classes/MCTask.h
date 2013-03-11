//
//  MCTask.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-27.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCTask__
#define __Military_Confrontation__MCTask__

#include "MCObject.h"
#include "JsonBox.h"

class MCTaskStatus;

/* 任务类别，对应category字段 */
enum {
    MCMainlineTask      = 1, /* 主线任务 */
    MCSideQuest         = 2, /* 支线任务 */
    MCMercenaryMission  = 3  /* 佣兵任务 */
};
typedef mc_enum_t MCTaskCategory;

extern const char *MCMainlineTaskName;
extern const char *MCSideQuestName;
extern const char *MCMercenaryMissionName;

const char *nameForMCTaskType(MCTaskCategory aTaskCategory);

class MCTask : public MCObject {
    friend class MCTaskAccessor;
public:
    void loadTask(JsonBox::Object &aTask);
    
    CC_SYNTHESIZE_READONLY(MCTaskCategory, taskCategory_, TaskCategory); /* 任务类别 */
    CC_SYNTHESIZE_READONLY(CCArray *, bonus_, Bonus);                    /* 任务奖励 */
    CC_SYNTHESIZE_READONLY(CCArray *, content_, Content);                /* 任务内容 */
    CC_SYNTHESIZE_READONLY(MCTaskStatus *, status_, Status);             /* 任务状态 */
};

#endif /* defined(__Military_Confrontation__MCTask__) */