#include "Gamescene.h"

Gamescene::Gamescene(Player *player){
	_me = PlayerOne::getInstance();
	_opponent = player;
}

Scene* Gamescene::createScene(Player *player)
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    auto layer = Gamescene::create(player);
    scene->addChild(layer);
    return scene;
}

cocos2d::LayerColor * Gamescene::create(Player *player)
{
	Gamescene *scene = new (std::nothrow) Gamescene(player);
	if (scene && scene->init()) {
		scene->autorelease();
		return scene;
	}
	CC_SAFE_DELETE(scene);
	return nullptr;
}

bool Gamescene::init()
{
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	LayerColor::initWithColor(Color4B(0.230f,0.230f,0.230f,1.0f));



	return true;
}
