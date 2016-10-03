#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include <iostream>
#include "Settings.h"

USING_NS_CC;

//static cocos2d::Size designResolutionSize = cocos2d::Size(960, 540);
static cocos2d::Size designResolutionSize = cocos2d::Size(1280, 720);

double atof(const char *nptr)
{
	return (strtod(nptr, NULL));
}

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
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("TCG3_12", Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("TCG3_12");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

	auto futils = FileUtils::getInstance();
	auto scrsize = glview->getFrameSize();
	std::vector<std::string> resDirOrders;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
resDirOrders.push_back("default");
#else
	if (scrsize.width >= 2560 || scrsize.height >= 2560) {
		resDirOrders.push_back("dpi-xhigh");
		resDirOrders.push_back("dpi-high");
		resDirOrders.push_back("dpi-medium");
		resDirOrders.push_back("dpi-low");
		glview->setDesignResolutionSize(2560,1600,ResolutionPolicy::NO_BORDER);
	}
	else if (scrsize.width >= 2048 || scrsize.height >= 2048) {
		resDirOrders.push_back("dpi-xhigh");
		resDirOrders.push_back("dpi-high");
		resDirOrders.push_back("dpi-medium");
		resDirOrders.push_back("dpi-low");
		glview->setDesignResolutionSize(2048, 1536, ResolutionPolicy::NO_BORDER);
	}
	else if (scrsize.width >= 1920 || scrsize.height >= 1920) {
		resDirOrders.push_back("dpi-high");
		resDirOrders.push_back("dpi-medium");
		resDirOrders.push_back("dpi-low");
		glview->setDesignResolutionSize(1920, 1080, ResolutionPolicy::NO_BORDER);
	}
	else if (scrsize.width >= 1280 || scrsize.height >= 1280) {
		resDirOrders.push_back("dpi-medium");
		resDirOrders.push_back("dpi-low");
		glview->setDesignResolutionSize(1280, 720, ResolutionPolicy::NO_BORDER);
	}
	else if (scrsize.width >= 1136 || scrsize.height >= 1136) {
		resDirOrders.push_back("dpi-medium");
		resDirOrders.push_back("dpi-low");
		glview->setDesignResolutionSize(1136, 640, ResolutionPolicy::NO_BORDER);
	}
	else if (scrsize.width >= 1024 || scrsize.height >= 1024) {
		resDirOrders.push_back("dpi-medium");
		resDirOrders.push_back("dpi-low");
		glview->setDesignResolutionSize(1024, 768, ResolutionPolicy::NO_BORDER);
	}
	else {
		resDirOrders.push_back("dpi-low");
		glview->setDesignResolutionSize(800, 480, ResolutionPolicy::NO_BORDER);
	}
#endif

	Size actualWinSize = director->getWinSizeInPixels();
	//director->setContentScaleFactor(actualWinSize.height / glview->getDesignResolutionSize().height);
	cocos2d::log("DRS => %.2f",director->getContentScaleFactor());
	futils->setSearchPaths(resDirOrders);
	//create a scene
    auto scene = HelloWorld::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
