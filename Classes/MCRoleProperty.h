//
//  MCRoleProperty.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-9.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef Military_Confrontation_MCRoleProperty_h
#define Military_Confrontation_MCRoleProperty_h

#include "MCType.h"

/**
 * MCRole 角色属性类型
 */
typedef mc_ushort_t mc_hp_t;   /* 生命值类型 */
typedef mc_ushort_t mc_pp_t;   /* 体力值类型 */

/**
 * 角色种族
 */
enum {
    MCTerrans       = MCMakeEnum(0), /* 人类 */
    MCTerrestrial   = MCMakeEnum(0), /* 陆行种 */
    MCFlying        = MCMakeEnum(1), /* 飞行种 */
    MCProtoss       = MCMakeEnum(2), /* 神 */
};
typedef mc_enum_t MCRoleRace;

/**
 * 角色状态
 */
enum {
    MCNormalState    = MCMakeEnum(0), /* 正常 */
    MCPoisonedState  = MCMakeEnum(1), /* 中毒 */
    MCChaosState     = MCMakeEnum(2), /* 混乱 */
    MCBlindingState  = MCMakeEnum(3), /* 盲 */
    MCVertigoState   = MCMakeEnum(4), /* 眩晕 */
    MCBurningState   = MCMakeEnum(5), /* 燃烧 */
    MCSlowState      = MCMakeEnum(6), /* 诅咒 */
    MCParalysisState = MCMakeEnum(7)  /* 麻痹 */
};
typedef mc_enum_t MCRoleState;

#endif
