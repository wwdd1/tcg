#include "Ladder.h"
#include "TMainMenu.h"
#include <iostream>
#include "PlayerOne.h"
#include "ClientManager.h"
#include <ui\CocosGUI.h>

USING_NS_CC;
using namespace std;
using namespace ui;

Scene* Ladderscene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = Ladderscene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool Ladderscene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	this->visibleSize = Director::getInstance()->getVisibleSize();
	this->origin = Director::getInstance()->getVisibleOrigin();

	LayerColor::initWithColor(Color4B(Color3B(30, 30, 30)));

	auto home_btn = Button::create("default/btn/home.png", "default/btn/home.png");
	this->addChild(home_btn);
	home_btn->setPosition(Vec2(visibleSize.width + origin.x - (home_btn->getContentSize().width*0.8f), visibleSize.height + origin.y - (home_btn->getContentSize().height*0.8f)));

	home_btn->addTouchEventListener([&,this](Ref* sender, Widget::TouchEventType type) {
		switch (type) {

		case cocos2d::ui::Widget::TouchEventType::ENDED:
			Director::getInstance()->popScene();
			break;
		}
	});

	auto client = ClientManager::getInstance();
	if (client->state_what() != STATE_ONLINE) {
		this->offlineMode();
	} else {
		this->onlineLadderSupply();
	}

	return true;
}

void Ladderscene::onlineLadderSupply()
{
	auto client = ClientManager::getInstance();
	REQUEST_T rq = _LADDER_;
	std::vector<TPLAYER_LADDER> players;
	client->request(rq, &players);

	std::vector<TPLAYER_LADDER>::iterator it = players.begin();
	int i = 1;
	while (it != players.end()) {
		TPLAYER_LADDER pl = *it;
		std::string text = pl.nickname;
		text += " Oo";
		auto lbl = Label::createWithTTF(text.c_str(), "fonts/OpenSans-Light.ttf", 25);
		lbl->setColor(Color3B::WHITE);
		this->addChild(lbl);
		lbl->setPosition(Vec2(visibleSize.width / 2 + origin.x, (visibleSize.height / 10)*i + origin.y));
		it++;
		i++;
	}
}

void Ladderscene::offlineMode()
{
	auto infomsg = Label::createWithTTF("You Are Not Connected To Server.", "fonts/OpenSans-Light.ttf",25);
	infomsg->setColor(Color3B::WHITE);
	this->addChild(infomsg);
	infomsg->setPosition(Vec2(visibleSize.width/2 + origin.x , visibleSize.height/2 + origin.y ));
}
