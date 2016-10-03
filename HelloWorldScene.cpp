#include "HelloWorldScene.h"
#include "TAlways.h"
#include "LocalDb.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
	
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	LayerColor::initWithColor(Color4B::WHITE);

	auto *splashCoverImg = Sprite::create("default/splashCover.png");
	splashCoverImg->setPosition(Vec2((origin.x + visibleSize.width/2),(origin.y + visibleSize.height/2)));
	this->addChild(splashCoverImg);

	this->scheduleOnce(schedule_selector(HelloWorld::goMainMenuScene),SPLASH_WAIT_TIME);

    return true;
}

void HelloWorld::goMainMenuScene(float dt) {
	Director::getInstance()->replaceScene(TransitionFade::create(0.5f, TAlways::createScene()));
}
