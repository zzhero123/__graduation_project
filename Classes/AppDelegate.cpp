#include "AppMacros.h"
#include "AppDelegate.h"
#include "script_support/CCScriptSupport.h"
#include "CCLuaEngine.h"

#include "MCRoleManager.h"

#include "MCTestbed.h"

#if MC_DEBUG_SERVER == 1
#include "MCSimpleGameSceneContextServer.h"
#endif

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
	CCSize frameSize = pEGLView->getFrameSize();

    pDirector->setOpenGLView(pEGLView);

    std::vector<std::string> searchPaths;

#if MC_ADAPTIVE_RESOLUTION == 1
    // Set the design resolution
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    pEGLView->setDesignResolutionSize(PCResource.size.width, PCResource.size.height, kResolutionExactFit);
    searchPaths.push_back(PCResource.directory);
//    pDirector->setContentScaleFactor(MIN(32 * 25 / PCResource.size.width, 32 * 15 / PCResource.size.height));
//    pDirector->setContentScaleFactor(0.75f);
    pDirector->setContentScaleFactor(MIN(32 * 25 / frameSize.width, 32 * 15 / frameSize.height));
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        // if the frame's height is larger than the height of medium resource size, select large resource.
	if (frameSize.height == smallResource.size.height) {
        pEGLView->setDesignResolutionSize(smallResource.size.width, smallResource.size.height, kResolutionNoBorder);
        searchPaths.push_back(smallResource.directory);
	}
        // if the frame's height is larger than the height of small resource size, select medium resource.
    else if (frameSize.height == mediumResource.size.height) {
        pEGLView->setDesignResolutionSize(mediumResource.size.width, mediumResource.size.height, kResolutionNoBorder);
        searchPaths.push_back(mediumResource.directory);
    }
        // if the frame's height is smaller than the height of medium resource size, select small resource.
	else {
        pEGLView->setDesignResolutionSize(largeResource.size.width, largeResource.size.height, kResolutionNoBorder);
		searchPaths.push_back(largeResource.directory);
    }
    pDirector->setContentScaleFactor(MIN(32 * 25 / frameSize.width, 32 * 15 / frameSize.height));
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    int i = 0;
    int size = sizeof(resources);
    int found = 0;
    for (; i < size; ++i) {
        if ((frameSize.width == resources[i].size.width && frameSize.height == resources[i].size.height)
            || (frameSize.width == resources[i].size.height && frameSize.height == resources[i].size.width)) {
            searchPaths.push_back(resources[i].directory);
            pEGLView->setDesignResolutionSize(resources[i].size.width, resources[i].size.height, kResolutionNoBorder);
            pDirector->setContentScaleFactor(MIN(32 * 25 / resources[i].size.width, 32 * 15 / resources[i].size.height));
            found = 1;
            break;
        }
    }
    if (found == 0) {
		searchPaths.push_back(resources[0].directory);
        pEGLView->setDesignResolutionSize(resources[0].size.width, resources[0].size.height, kResolutionNoBorder);
        pDirector->setContentScaleFactor(MIN(32 * 25 / resources[0].size.width, 32 * 15 / resources[0].size.height));
    }
#else
    // if the frame's height is larger than the height of medium resource size, select large resource.
	if (frameSize.height > mediumResource.size.height) {
        searchPaths.push_back(largeResource.directory);
        pEGLView->setDesignResolutionSize(largeResource.size.width, largeResource.size.height);
        pDirector->setContentScaleFactor(MIN(largeResource.size.height/smallResource.size.height, largeResource.size.width/smallResource.size.width));
	}
    // if the frame's height is larger than the height of small resource size, select medium resource.
    else if (frameSize.height > smallResource.size.height) {
        searchPaths.push_back(mediumResource.directory);
        pEGLView->setDesignResolutionSize(mediumResource.size.width, mediumResource.size.height);
        pDirector->setContentScaleFactor(MIN(mediumResource.size.height/smallResource.size.height, mediumResource.size.width/smallResource.size.width));
    }
    // if the frame's height is smaller than the height of medium resource size, select small resource.
	else {
		searchPaths.push_back(smallResource.directory);
        pEGLView->setDesignResolutionSize(smallResource.size.width, smallResource.size.height);
        pDirector->setContentScaleFactor(MIN(smallResource.size.height, smallResource.size.width));
    }
#endif // platforms
#else
    CCSize designSize = CCSizeMake(960, 640);
    
    searchPaths.push_back(CommonResource.directory);
    if (frameSize.height > 1280) {
        CCSize resourceSize = CCSizeMake(2048, 1536);
        std::vector<std::string> searchPaths;
//        searchPaths.push_back("hd"); //没资源
        pDirector->setContentScaleFactor(0.75 * resourceSize.height / designSize.height);
    } else {
        pDirector->setContentScaleFactor(0.75);
    }
    
    CCEGLView::sharedOpenGLView()->setDesignResolutionSize(designSize.width, designSize.height, kResolutionFixedWidth);
#endif // MC_ADAPTIVE_RESOLUTION == 0
    CCFileUtils::sharedFileUtils()->setSearchPaths(searchPaths);
	
    // turn on display FPS
//    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);
    
    CCLog("%s(%d): %.0f %.0f %.0f", __FILE__, __LINE__, frameSize.width, frameSize.height, pDirector->getContentScaleFactor());
    
    // register lua engine
    CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();
    CCScriptEngineManager::sharedManager()->setScriptEngine(pEngine);
    
#if MC_DEBUG_SERVER == 1
    MCSimpleGameSceneContextServer *server = MCSimpleGameSceneContextServer::defaultSimpleGameSceneContextServer();
    server->setPort(kMCListeningPort);
    server->runloop();
#endif
    
    // run
//    pDirector->pushScene(MCMainMenuLayer::scene());
//    pDirector->runWithScene(MCSplashLayer::scene());
    
    pDirector->runWithScene(MCTestbed::scene());

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
