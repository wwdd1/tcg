#include "TMainMenu.h"

USING_NS_CC;
using namespace std;
using namespace ui;

Scene* TMainMenu::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = TMainMenu::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool TMainMenu::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

	LayerColor::initWithColor(Color4B(Color3B(30,30,30)));
	
	if (!this->createMenu()) {
		CCLOG("error on creating main menu.");
	}

    return true;
}

bool TMainMenu::createMenu() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto player = PlayerOne::getInstance();

#define BACKGROUND_SPEED 120 //as seconds
	//background image implementation
	auto image = Sprite::create("default/bg/bg-red_stadium.png");
	image->setPosition(Vec2(0,visibleSize.height/2 + origin.y));
	image->setAnchorPoint(Vec2(0.5f,0.5f));
	float imaxx = image->getBoundingBox().getMaxX();
	this->addChild(image, 1);
	auto sequence = Sequence::create(MoveBy::create(BACKGROUND_SPEED,Vec2(imaxx, 0)),MoveBy::create(BACKGROUND_SPEED,Vec2(-imaxx,0)),nullptr);
	image->runAction(RepeatForever::create(sequence));

	//left gray info box cover
	auto info_cover = LayerColor::create(Color4B(Color3B(20,20,20)));
	info_cover->setContentSize(Size(visibleSize.width/3,visibleSize.height/1.7f));
	this->addChild(info_cover,2);
	info_cover->ignoreAnchorPointForPosition(false);
	info_cover->setAnchorPoint(Vec2(0.5f,0.5f));
	info_cover->setPosition(Vec2(origin.x + info_cover->getContentSize().width * 0.8f,visibleSize.height/2 + origin.y));

	//player's nickname label
	auto lblinfo_uname = Label::createWithTTF(player->get_username(), "fonts/OpenSans-Light.ttf", 30);
	info_cover->addChild(lblinfo_uname);
	lblinfo_uname->setPosition(Vec2(info_cover->getContentSize().width/2,info_cover->getContentSize().height - lblinfo_uname->getContentSize().height*0.8));
	
	//line below nickname
	auto line = DrawNode::create();
	line->drawLine(Vec2(20, lblinfo_uname->getBoundingBox().getMinY() -5), Vec2(info_cover->getContentSize().width -20, lblinfo_uname->getBoundingBox().getMinY() - 5), Color4F(Color3B(80,80,80)));
	line->setContentSize(Size(30,5));
	info_cover->addChild(line); 


	//ladder button inside infobox
	auto btn = ui::Button::create("default/btn/btn.png","default/btn/btnc.png");
	btn->setTitleText("Ladder");
	btn->setTitleColor(Color3B::WHITE);
	btn->setTitleFontName("fonts/OpenSans-Light.ttf");
	btn->setTitleFontSize(25);
	btn->setAnchorPoint(Vec2(0.5f,0.5f));
	info_cover->addChild(btn);
	
	
	//inventory button above ladder.
	auto btninventory = ui::Button::create("default/btn/btninventory.png", "default/btn/btninventoryc.png");
	btninventory->setTitleText("Inventory");
	btninventory->setTitleColor(Color3B::WHITE);
	btninventory->setTitleFontName("fonts/OpenSans-Light.ttf");
	btninventory->setTitleFontSize(25);
	btninventory->setAnchorPoint(Vec2(0.5f,0.5f));
	info_cover->addChild(btninventory);
	
	//positioning buttons inside infobox.
	btn->setPosition(Vec2(info_cover->getContentSize().width / 2 ,btn->getContentSize().height*0.8f)); //btn half height + btn half height*0.3*1
	btninventory->setPosition(Vec2(info_cover->getContentSize().width / 2 , btn->getContentSize().height*2.1f)); //btn half height + btn half height*0.3*2

	//experience points bar background pattern image.
	auto xp_bar = ProgressTimer::create(Sprite::create("default/pbar.png"));
	Size xp_bar_bg_size = xp_bar->getContentSize();
	xp_bar->setType(ProgressTimer::Type::BAR);
	xp_bar->setMidpoint(Vec2(0,0));
	xp_bar->setBarChangeRate(Vec2(1,0));
	xp_bar->setPercentage(82);

	//xp information text
	std::string xp_percmeter = player->toString(player->get_xp()) + " / 1410";
	auto xp_point_above_bar_text = Label::createWithTTF(xp_percmeter.c_str(),"fonts/OpenSans-Semibold.ttf",15*0.75);
	xp_point_above_bar_text->setAnchorPoint(Vec2(0.5f,0.5f));
	info_cover->addChild(xp_point_above_bar_text);

	//level information text
	std::string level_meter = "Level " + player->toString(player->get_level());
	auto level_above_bar_text = Label::createWithTTF(level_meter.c_str(), "fonts/OpenSans-Semibold.ttf",30);
	level_above_bar_text->setAnchorPoint(Vec2(0,0.5f));
	info_cover->addChild(level_above_bar_text);

	//level badge
	auto level_badge = player->get_level_badge(); //sprite.
	level_above_bar_text->addChild(level_badge);
	level_badge->setPosition(Vec2(level_above_bar_text->getContentSize().width + level_badge->getContentSize().width,level_above_bar_text->getContentSize().height/2));

	//dark gray layer behind xp bar pattern image.
	auto xp_bar_bg = LayerColor::create(Color4B(Color3B(10, 10, 10)));
	xp_bar_bg->ignoreAnchorPointForPosition(false);
	xp_bar_bg->setAnchorPoint(Vec2(0.5f, 0.5f));
	xp_bar_bg->setContentSize(Size(xp_bar_bg_size.width +10 ,xp_bar_bg_size.height*1.3f));
	xp_bar_bg->addChild(xp_bar);
	xp_bar->setPosition(Vec2(xp_bar_bg->getContentSize().width/2,xp_bar_bg->getContentSize().height/2));
	info_cover->addChild(xp_bar_bg);
	xp_bar_bg->setPosition(Vec2(info_cover->getContentSize().width / 2, lblinfo_uname->getBoundingBox().getMinY() - lblinfo_uname->getContentSize().height - xp_point_above_bar_text->getContentSize().height));
	xp_point_above_bar_text->setPosition(Vec2(info_cover->getContentSize().width / 2, xp_bar_bg->getBoundingBox().getMaxY() + xp_point_above_bar_text->getContentSize().height));
	//win loss counts as string on screen
	float mid_area_mid_y_point_on_left_box = btninventory->getBoundingBox().getMaxY() + ((xp_bar_bg->getBoundingBox().getMinY() - btninventory->getBoundingBox().getMaxY())/2);
	level_above_bar_text->setPosition(Vec2(btninventory->getBoundingBox().getMinX(), mid_area_mid_y_point_on_left_box));


	//x position measuring for store and play buttons
	float right_side_center_point = (visibleSize.width - info_cover->getBoundingBox().getMaxX()) / 3;
	
	//store button
	auto store_btn = ui::Button::create("default/btn/pack-store.png", "default/btn/pack-storec.png");
	store_btn->setTitleText("STORE");
	store_btn->setTitleAlignment(TextHAlignment::RIGHT);
	store_btn->setTitleColor(Color3B::WHITE);
	store_btn->setTitleFontName("fonts/OpenSans-ExtraBoldItalic.ttf");
	store_btn->setTitleFontSize(40);
	this->addChild(store_btn, 3, "store_btn");
	store_btn->setPosition(Vec2(info_cover->getBoundingBox().getMaxX() + right_side_center_point*1.0f + origin.x, visibleSize.height / 2 + origin.y));
	


	//play button
	auto play_btn = ui::Button::create("default/btn/playbtn.png","default/btn/playbtnc.png");
	play_btn->setTitleText("PLAY");
	play_btn->setTitleColor(Color3B::WHITE);
	play_btn->setTitleFontName("fonts/OpenSans-ExtraBoldItalic.ttf");
	play_btn->setTitleFontSize(50);
	this->addChild(play_btn,4,"play_btn");
	play_btn->setPosition(Vec2(info_cover->getBoundingBox().getMaxX() + right_side_center_point*2.0f + origin.x , visibleSize.height/2 + origin.y));

	//coin o'meter
	auto coin_layer = LayerColor::create(Color4B(Color3B(242, 278, 71)));
	auto coin_layer_icon = Sprite::create("default/icon/coin.png");
	auto coin_label = Label::createWithTTF(player->toString(player->get_tp()), "fonts/OpenSans-Light.ttf", 30*0.75);
	coin_label->setColor(Color3B::WHITE);
	coin_layer->setContentSize(Size(coin_layer_icon->getContentSize().width + coin_label->getContentSize().width + 20, coin_layer_icon->getContentSize().height));
	this->addChild(coin_layer, 5);
	coin_layer->setPosition(Vec2(info_cover->getBoundingBox().getMinX(), info_cover->getBoundingBox().getMaxY() + 10));
	coin_layer->addChild(coin_layer_icon);
	coin_layer->addChild(coin_label);
	coin_layer_icon->setAnchorPoint(Vec2(0, 0.5f));
	coin_layer_icon->setPosition(Vec2(0, coin_layer->getContentSize().height / 2));
	coin_label->setAnchorPoint(Vec2(1.0f, 0.5f));
	coin_label->setPosition(Vec2(coin_layer->getContentSize().width - 10, coin_layer->getContentSize().height / 2));

	
	//friends button
	auto friends_layer = LayerColor::create(Color4B(Color3B(44, 167, 218)));
	auto friends_layer_icon = Sprite::create("default/icon/friendship.png");
	friends_layer->setContentSize(Size(info_cover->getContentSize().width - coin_layer->getContentSize().width -10,coin_layer->getContentSize().height));
	this->addChild(friends_layer,6);
	friends_layer->setPosition(Vec2(coin_layer->getBoundingBox().getMaxX()+10,coin_layer->getPosition().y));
	friends_layer->addChild(friends_layer_icon);
	friends_layer_icon->setAnchorPoint(Vec2(0.5f, 0.5f));
	friends_layer_icon->setPosition(Vec2(friends_layer->getContentSize().width / 2, friends_layer->getContentSize().height / 2));

	//event listeners for buttons
	btn->addTouchEventListener([&] (Ref* sender, ui::Widget::TouchEventType type){
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::ENDED:
			Director::getInstance()->pushScene(TransitionMoveInL::create(0.3f, Ladderscene::createScene()));
			break;
		default:
			break;
		}
	});
	btninventory->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::ENDED:
			Director::getInstance()->pushScene(TransitionMoveInB::create(0.3f, Inventoryscene::createScene()));
			break;
		default:
			break;
		}
	});
	play_btn->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::ENDED:
			this->startTheGame();
			break;
		default:
			break;
		}
	});

	store_btn->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::ENDED:
			Director::getInstance()->pushScene(TransitionMoveInT::create(0.5f,MarketPlace::createScene()));
			break;
		default:
			break;
		}
	});

	return true;
}

void TMainMenu::startTheGame() {
	CCLOG("PLAY CLICKED.");
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	auto adapter = GameStartAdapter::createScene();
	//DONT FORGET TO ASSIGN GAMETYPE
	GameStartAdapter::gametype = GAMETYPE::SOLO_GAME;
	Director::getInstance()->replaceScene(TransitionFade::create(0.1f,adapter));
}

void TMainMenu::exitApp() {	
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
