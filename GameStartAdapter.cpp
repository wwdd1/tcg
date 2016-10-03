#include "GameStartAdapter.h"

GAMETYPE GameStartAdapter::gametype;

Scene* GameStartAdapter::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameStartAdapter::create();
	
	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool GameStartAdapter::init()
{
	// 1. super init first
	if (!LayerColor::init())
	{
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	LayerColor::initWithColor(Color4B(0.100f, 0.230f, 0.022f, 1.0f));

	//return to main menu button
	auto home_btn = ui::Button::create("default/btn/home.png", "default/btn/home.png");
	this->addChild(home_btn, 9999);
	home_btn->setPosition(Vec2(visibleSize.width + origin.x - (home_btn->getContentSize().width*0.8f), visibleSize.height + origin.y - (home_btn->getContentSize().height*0.8f)));

	home_btn->addTouchEventListener([&, this](Ref* sender, ui::Widget::TouchEventType type) {
		switch (type) {
		case cocos2d::ui::Widget::TouchEventType::ENDED:
			Director::getInstance()->replaceScene(TransitionFade::create(0.2f,TMainMenu::createScene()));
			break;
		}
	});

	if (gametype == GAMETYPE::SOLO_GAME) {
		this->soloGame();
	}else
		if (gametype == GAMETYPE::RANKED_GAME) {
			this->rankedGame();
		}

	return true;
}

/**
*	@SOLO GAME SCENE
*/
void GameStartAdapter::soloGame()
{
	GameStartAdapter *scene = this;
	ui::ScrollView *game_modes_scroller = ui::ScrollView::create();
	int SOLO_MODES_WE_HAVE = 3;
	
	auto btn_casual = ui::Button::create("default/btn/btn-casual.png", "default/btn/btn-casual.png");
	auto btn_hard = ui::Button::create("default/btn/btn-hard.png", "default/btn/btn-hard.png");
	auto btn_impossible = ui::Button::create("default/btn/btn-impossible.png", "default/btn/btn-impossible.png");
	
	//btn_casual preparing
	auto btn_casual_label = Label::createWithTTF("CASUAL", "fonts/OpenSans-ExtraBoldItalic.ttf", 40);
	btn_casual->addChild(btn_casual_label);
	btn_casual_label->setPosition(Vec2(btn_casual->getContentSize().width/2,btn_casual->getContentSize().height/2));
	btn_casual_label->enableOutline(Color4B::BLACK, 3);
	auto btn_casual_icon = Sprite::create("default/icon/soccerball.png");
	btn_casual->addChild(btn_casual_icon);
	btn_casual_icon->setAnchorPoint(Vec2(0.5f,0));
	btn_casual_icon->setPosition(Vec2(btn_casual_label->getBoundingBox().getMidX(),btn_casual_label->getBoundingBox().getMaxY() + 5));
	auto btn_casual_info_label = Label::createWithTTF("Beat 5 rivals in a row then win a card!", "fonts/OpenSans-Regular.ttf", 15);
	btn_casual_info_label->setColor(Color3B::BLACK);
	btn_casual_info_label->setMaxLineWidth(150);
	btn_casual_info_label->setAlignment(TextHAlignment::CENTER);
	btn_casual_info_label->setAnchorPoint(Vec2(0.5f,1.0f));
	btn_casual->addChild(btn_casual_info_label);
	btn_casual_info_label->setPosition(Vec2(btn_casual_label->getBoundingBox().getMidX(),btn_casual_label->getBoundingBox().getMinY() - 5));

	//btn_casual preparing
	auto btn_hard_label = Label::createWithTTF("HARD", "fonts/OpenSans-ExtraBoldItalic.ttf", 40);
	btn_hard->addChild(btn_hard_label);
	btn_hard_label->setPosition(Vec2(btn_hard->getContentSize().width / 2, btn_hard->getContentSize().height / 2));
	btn_hard_label->enableOutline(Color4B::BLACK, 3);
	auto btn_hard_icon = Sprite::create("default/icon/fancysoccerball.png");
	btn_hard->addChild(btn_hard_icon);
	btn_hard_icon->setAnchorPoint(Vec2(0.5f, 0));
	btn_hard_icon->setPosition(Vec2(btn_hard_label->getBoundingBox().getMidX(), btn_hard_label->getBoundingBox().getMaxY() + 5));
	auto btn_hard_info_label = Label::createWithTTF("Beat 8 rivals in a row then win a card! Also earn extra TP per win.", "fonts/OpenSans-Regular.ttf", 15);
	btn_hard_info_label->setColor(Color3B::WHITE);
	btn_hard_info_label->setMaxLineWidth(150);
	btn_hard_info_label->setAlignment(TextHAlignment::CENTER);
	btn_hard_info_label->setAnchorPoint(Vec2(0.5f, 1.0f));
	btn_hard->addChild(btn_hard_info_label);
	btn_hard_info_label->setPosition(Vec2(btn_hard_label->getBoundingBox().getMidX(), btn_hard_label->getBoundingBox().getMinY() - 5));

	//btn_casual preparing
	auto btn_impossible_label = Label::createWithTTF("IMPOSSIBLE", "fonts/OpenSans-ExtraBoldItalic.ttf", 40);
	btn_impossible->addChild(btn_impossible_label);
	btn_impossible_label->setPosition(Vec2(btn_impossible->getContentSize().width / 2, btn_impossible->getContentSize().height / 2));
	btn_impossible_label->enableOutline(Color4B::BLACK, 3);
	auto btn_impossible_icon = Sprite::create("default/icon/firesoccerball.png");
	btn_impossible->addChild(btn_impossible_icon);
	btn_impossible_icon->setAnchorPoint(Vec2(0.5f, 0));
	btn_impossible_icon->setPosition(Vec2(btn_impossible_label->getBoundingBox().getMidX(), btn_impossible_label->getBoundingBox().getMaxY() + 5));
	auto btn_impossible_info_label = Label::createWithTTF("Beat 10 rivals in a row then win a card! Also earn extra TP and XP per win.", "fonts/OpenSans-Regular.ttf", 15);
	btn_impossible_info_label->setColor(Color3B::WHITE);
	btn_impossible_info_label->setMaxLineWidth(150);
	btn_impossible_info_label->setAlignment(TextHAlignment::CENTER);
	btn_impossible_info_label->setAnchorPoint(Vec2(0.5f, 1.0f));
	btn_impossible->addChild(btn_impossible_info_label);
	btn_impossible_info_label->setPosition(Vec2(btn_impossible_label->getBoundingBox().getMidX(), btn_impossible_label->getBoundingBox().getMinY() - 5));

	//settings for scroller :
	game_modes_scroller->setDirection(ui::ScrollView::Direction::HORIZONTAL);
	//using btn_casual as delegate to size scroller.
	game_modes_scroller->setContentSize(Size((btn_casual->getContentSize().width + 10) * (SOLO_MODES_WE_HAVE) - 10,btn_casual->getContentSize().height));
	game_modes_scroller->setInnerContainerSize(btn_casual->getContentSize());
	game_modes_scroller->setBounceEnabled(true);
	game_modes_scroller->setAnchorPoint(Vec2(0.5f,0.5f));
	scene->addChild(game_modes_scroller);
	game_modes_scroller->setPosition(Vec2(visibleSize.width/2 + origin.x , visibleSize.height/2 + origin.y));

	game_modes_scroller->addChild(btn_casual);
	btn_casual->setPosition(Vec2(btn_casual->getContentSize().width/2,btn_casual->getContentSize().height/2));
	btn_casual->addTouchEventListener(CC_CALLBACK_2(GameStartAdapter::TouchEventCasual,this));

	game_modes_scroller->addChild(btn_hard);
	btn_hard->setPosition(Vec2(btn_casual->getBoundingBox().getMaxX() + 10 + btn_hard->getContentSize().width / 2,btn_hard->getContentSize().height/2));
	btn_hard->addTouchEventListener(CC_CALLBACK_2(GameStartAdapter::TouchEventHard, this));

	
	game_modes_scroller->addChild(btn_impossible);
	btn_impossible->setPosition(Vec2(btn_hard->getBoundingBox().getMaxX() + 10 + btn_impossible->getContentSize().width / 2, btn_impossible->getContentSize().height / 2));
	btn_impossible->addTouchEventListener(CC_CALLBACK_2(GameStartAdapter::TouchEventImpossible, this));

	//select a game mode
	auto header = Label::createWithTTF("Select a game mode:","fonts/OpenSans-ExtraBoldItalic.ttf",25);
	scene->addChild(header);
	header->setAnchorPoint(Vec2(0,0));
	header->setPosition(Vec2(game_modes_scroller->getBoundingBox().getMinX() + origin.x,game_modes_scroller->getBoundingBox().getMaxY() + origin.y + 10));


}

void GameStartAdapter::TouchEventCasual(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType type)
{
	switch (type) {
	case ui::Widget::TouchEventType::ENDED: {
		auto easies = new std::vector<std::string>();
		auto normals = new std::vector<std::string>();
		auto hards = new std::vector<std::string>();

		easies->push_back("Vertigo");
		easies->push_back("Xenon");

		normals->push_back("Krolle");

		hards->push_back("Carl JR");
		hards->push_back("Frost Beule");

		auto opponent_tower = new std::map<DIFFICULTY,std::vector<std::string> *>();
		opponent_tower->insert(std::pair<DIFFICULTY, std::vector<std::string> *>(DIFFICULTY::EASY, easies));
		opponent_tower->insert(std::pair<DIFFICULTY, std::vector<std::string> *>(DIFFICULTY::NORMAL, normals));
		opponent_tower->insert(std::pair<DIFFICULTY, std::vector<std::string> *>(DIFFICULTY::HARD, hards));


		/**		**USAGE**
		*	FIRST create @reviewsceneobject then make assignments thru @set methods.
		*	SECOND if you sure you set all needed game data then replace scene with @reviewsceneobject
		*/

		auto reviewsceneobject = GameStartReview::createScene(SOLOGAMETYPE::CASUAL,opponent_tower);
		Director::getInstance()->replaceScene(TransitionFade::create(0.1f,reviewsceneobject));
	}break;
	}
}

void GameStartAdapter::TouchEventHard(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType type)
{
	switch (type) {
	case ui::Widget::TouchEventType::ENDED: {
		auto easies = new std::vector<std::string>();
		auto normals = new std::vector<std::string>();
		auto hards = new std::vector<std::string>();

		easies->push_back("Kimi");
		easies->push_back("Beatles");
		easies->push_back("Ensar");

		normals->push_back("Sabre");
		normals->push_back("Poker");
	
		hards->push_back("Karjen");
		hards->push_back("HakkiJunior");
		hards->push_back("Spam");

		auto opponent_tower = new std::map<DIFFICULTY, std::vector<std::string> *>();
		opponent_tower->insert(std::pair<DIFFICULTY, std::vector<std::string> *>(DIFFICULTY::EASY, easies));
		opponent_tower->insert(std::pair<DIFFICULTY, std::vector<std::string> *>(DIFFICULTY::NORMAL, normals));
		opponent_tower->insert(std::pair<DIFFICULTY, std::vector<std::string> *>(DIFFICULTY::HARD, hards));


		/**		**USAGE**
		*	FIRST create @reviewsceneobject then make assignments thru @set methods.
		*	SECOND if you sure you set all needed game data then replace scene with @reviewsceneobject
		*/

		auto reviewsceneobject = GameStartReview::createScene(SOLOGAMETYPE::HARD, opponent_tower);
		Director::getInstance()->replaceScene(TransitionFade::create(0.1f, reviewsceneobject));
	}break;
	}

}

void GameStartAdapter::TouchEventImpossible(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType type)
{
	switch (type) {
	case ui::Widget::TouchEventType::ENDED: {
		auto easies = new std::vector<std::string>();
		auto normals = new std::vector<std::string>();
		auto hards = new std::vector<std::string>();

		easies->push_back("ViperSix");
		easies->push_back("Brian");
		easies->push_back("Santana");

		normals->push_back("Sabre");
		normals->push_back("Mesko");
		
		hards->push_back("Wally");
		hards->push_back("Nevermind");
		hards->push_back("YoYo");
		hards->push_back("Massa");
		hards->push_back("Kronos");

		auto opponent_tower = new std::map<DIFFICULTY, std::vector<std::string> *>();
		opponent_tower->insert(std::pair<DIFFICULTY, std::vector<std::string> *>(DIFFICULTY::EASY, easies));
		opponent_tower->insert(std::pair<DIFFICULTY, std::vector<std::string> *>(DIFFICULTY::NORMAL, normals));
		opponent_tower->insert(std::pair<DIFFICULTY, std::vector<std::string> *>(DIFFICULTY::HARD, hards));


		/**		**USAGE**
		*	FIRST create @reviewsceneobject then make assignments thru @set methods.
		*	SECOND if you sure you set all needed game data then replace scene with @reviewsceneobject
		*/

		auto reviewsceneobject = GameStartReview::createScene(SOLOGAMETYPE::IMPOSSIBLE, opponent_tower);
		Director::getInstance()->replaceScene(TransitionFade::create(0.1f, reviewsceneobject));
	}break;
	}
}

/**
*	@ONLINE GAME SCENE
*/
void GameStartAdapter::rankedGame()
{
}
