#include "AppDelegate.h"
#include "StartScene.h"
#include "SoundManager.h"
#include "Const.h"
#include "GameScene.h"
#include "ServerDataSwap.h"
#include "BuildingUILayer.h"
#include "MixSuggestLayer.h"
#ifdef ANALYTICS
#include "MobClickCpp.h"
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "UnityAdsMana.h"
#include "IOSPurchaseWrap.h"
#include "iosfunc.h"
#endif

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(720, 1280);
static cocos2d::Size smallResolutionSize = cocos2d::Size(540, 960);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(720, 1280);
static cocos2d::Size largeResolutionSize = cocos2d::Size(1080, 1920);

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
        glview = GLViewImpl::createWithRect("JH", Rect(0, 0, 540, 960));
#else
        glview = GLViewImpl::create("JH");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);

    // Set the design resolution
	glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::EXACT_FIT);
    //Size frameSize = glview->getFrameSize();
    //// if the frame's height is larger than the height of medium size.
    //if (frameSize.height > mediumResolutionSize.height)
    //{        
    //    director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
    //}
    //// if the frame's height is larger than the height of small size.
    //else if (frameSize.height > smallResolutionSize.height)
    //{        
    //    director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
    //}
    //// if the frame's height is smaller than the height of medium size.
    //else
    //{        
    //    director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
    //}

    register_all_packages();

	GlobalData::init();
	//
	GlobalData::loadAllResourceJsonData();
	//读取角色配置文件
	GlobalData::loadHeroAtrJsonData();

	//读取每个建筑配置文件
	GlobalData::loadBuildActionJSon();

	//读取资源配置文件
	GlobalData::loadResJsonData();

	//读取内功，外功配置文件
	GlobalData::loadWG_NGJsonData();

	//读取武器防具配置文件
	GlobalData::loadEquipJsonData();

	//读取组合功法配置文件
	GlobalData::loadMixGfJsonData();

#if defined(CC_PLATFORM_IOS) && defined(ANALYTICS)
    MOBCLICKCPP_START_WITH_APPKEY_AND_CHANNEL("59264ff476661347e2000897", "jh1");
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //initAds();
    initBuy();
#endif	

	SoundManager::getInstance()->loadSounds();
    // create a scene. it's an autorelease object
	auto scene = StartScene::createScene();
    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    SoundManager::getInstance()->saveVolume();
    SoundManager::getInstance()->setVolume(0.0f);
#else
	SoundManager::getInstance()->pauseBackMusic();
#endif
	
	if (g_gameLayer != NULL)
	{
		g_gameLayer->saveAllData();
	}

	if (GlobalData::isOnline)
		ServerDataSwap::init(NULL)->postOneData(GlobalData::getUId());
    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    SoundManager::getInstance()->setVolume(SoundManager::getInstance()->getSaveVolume());
#else
	SoundManager::getInstance()->resumeBackMusic();
#endif
	if (g_gameLayer != NULL)
	{
		BuildingUILayer* blayer = (BuildingUILayer*)g_gameLayer->getChildByName("builduilayer");
		MixSuggestLayer* mlayer = (MixSuggestLayer*)g_gameLayer->getChildByName("mixsuggestlayer");
		if (blayer == NULL)
		{
			if (mlayer != NULL)
			{
				if (GlobalData::isOnline)
					mlayer->getServerTime();
			}
			else
			{
				if (g_hero != NULL && !ServerDataSwap::isGetingData())
				{
					if (GlobalData::isOnline)
					{
						ServerDataSwap::init(g_gameLayer)->vipIsOn(g_hero->getHeadID());
						ServerDataSwap::init(NULL)->getFactionList();
					}
				}
			}
		}
		else
		{
			if (GlobalData::isOnline)
				blayer->getServerTime();
		}
	}
    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
