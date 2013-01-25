//
//  MCControllerLayer.h
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-23.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Military_Confrontation__MCControllerLayer__
#define __Military_Confrontation__MCControllerLayer__

#include "MCControllerDelegate.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "MCJoypad.h"
#endif /* Android和iOS控制响应 */
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
#include "MCKeyboard.h"
#include "MCJoypad.h"
#endif /* MacOSX控制响应 */

class MCControllerLayer : public CCLayer {
    
public:
    bool init();
    
    CREATE_FUNC(MCControllerLayer);
    
private:
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    MCJoypad *joypad_;
#endif /* Android和iOS控制响应 */
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    MCKeyboard *keybord_;
    MCJoypad *joypad_;
#endif /* MacOSX控制响应 */
    
    CC_PROPERTY(MCControllerDelegate *, delegate_, Delegate);
};

#endif /* defined(__Military_Confrontation__MCControllerLayer__) */