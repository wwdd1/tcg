#include "GameStartReviewScene.h"

void GameStartReview::TouchEventPlay(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType type)
{
	switch (type) {
	case ui::Widget::TouchEventType::ENDED: {
		Director::getInstance()->replaceScene(Gamescene::createScene());
	}break;
	}
}

Scene* GameStartReview::createScene(SOLOGAMETYPE sologametype, std::map<DIFFICULTY, std::vector<std::string> *> *rivals, int towerIndex,Player *opponent)
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto gsr_delegate = GameStartReview::create();
	
	//statement sequence below must be made before scene->addChild() method
	gsr_delegate->set_towerIndex(towerIndex);
	gsr_delegate->set_towerMap(rivals);
	gsr_delegate->set_soloGameType(sologametype);
	gsr_delegate->drawSoloScene();

	// add layer as a child to scene
	scene->addChild(gsr_delegate);

	// return the scene
	return scene;
}

GameStartReview *GameStartReview::create()
{
	GameStartReview *ret = new (std::nothrow) GameStartReview();
	if (ret && ret->init())
	{
		ret->autorelease();
		return ret;
	}
	else
	{
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
}

bool GameStartReview::init()
{
	// 1. super init first
	if (!LayerColor::init())
	{
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	LayerColor::initWithColor(Color4B::BLACK);

	//return to main menu button
	auto home_btn = ui::Button::create("default/btn/home.png", "default/btn/home.png");
	this->addChild(home_btn, 9999);
	home_btn->setPosition(Vec2(visibleSize.width + origin.x - (home_btn->getContentSize().width*0.8f), visibleSize.height + origin.y - (home_btn->getContentSize().height*0.8f)));

	home_btn->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
		switch (type) {
		case cocos2d::ui::Widget::TouchEventType::ENDED:
			Director::getInstance()->replaceScene(TransitionFade::create(0.2f, TMainMenu::createScene()));
			break;
		}
	});

	return true;
}
/*
void GameStartReview::onEnter() {
	//check if necessary data assigned or not
	drawSoloScene();
}*/

void GameStartReview::drawSoloScene()
{
	this->setColor(Color3B(30.0f, 30.0f, 30.0f));
	Color3B fontcolor = Color3B::WHITE;
	switch (sologametype)
	{
	case SOLOGAMETYPE::CASUAL: {
		opponentCount = 5;
	}break;
	case SOLOGAMETYPE::HARD: {
		opponentCount = 8;
	}break;
	case SOLOGAMETYPE::IMPOSSIBLE: {
		opponentCount = 10;
	}break;
	}

	if (opponentCount == 0 && towerIndex < 0) {
		return;
	}

	//which opponent's turn from tower
	DIFFICULTY current_difficulty;
	PlayerAI *opponent = createOpponent();
	if (opponent == nullptr)	return;
	
	//tower element positioning
	float tower_height = visibleSize.height/1.5f;
	float tower_y_padding = tower_height / opponentCount;
	float tower_start_pos = (visibleSize.height - tower_height)/2;

	int elm_seq = 0;	//IMPORTANT : in case of changing elm_seq difference from 0, notice the if statement inside second for below. (elm_seq == towerIndex)
	int x_coord;	//x coordinate of single tower player.
	std::vector<Vec2> elm_positions;
	DIFFICULTY dct_array[] = { DIFFICULTY::EASY , DIFFICULTY::NORMAL , DIFFICULTY::HARD };
	for (int i = 0; i < 3; i++) {// 3 means dct_array size
		DIFFICULTY dct = dct_array[i];
		auto names = towerMap->at(dct);

		std::string anopp_imagedir;
		switch (dct)
		{
		case DIFFICULTY::EASY:
			anopp_imagedir = "default/icon/opponent_easy.png";
			break;
		case DIFFICULTY::NORMAL:
			anopp_imagedir = "default/icon/opponent_normal.png";
			break;
		case DIFFICULTY::HARD:
			anopp_imagedir = "default/icon/opponent_hard.png";
			break;
		}

		for (int j = 0; j < names->size(); j++) {
			Sprite *anopponent = Sprite::create(anopp_imagedir.c_str());
			x_coord = (elm_seq % 2 == 0) ? visibleSize.width/6 : visibleSize.width/4 ;
			anopponent->setPosition(Vec2(x_coord + origin.x, origin.y + tower_start_pos + elm_seq*(tower_y_padding)));
			elm_positions.push_back(anopponent->getPosition());
			this->addChild(anopponent , 999);

			//animate current opponent at tower.
			if (elm_seq == towerIndex) {
				auto anim_seq = Sequence::create(ScaleTo::create(0.5f, 1.2f), ScaleTo::create(0.5f, 1.0f), nullptr);
				anopponent->runAction(RepeatForever::create(anim_seq));
				//and we can get current difficulty tho
				current_difficulty = dct;
			}

			Label *oppLabel = Label::create(names->at(j).c_str(), "fonts/OpenSans-Regular.ttf", 18);
			oppLabel->setColor(fontcolor);
			anopponent->addChild(oppLabel);
			oppLabel->setAnchorPoint(Vec2(0,0.5f));
			oppLabel->setPosition(Vec2(anopponent->getContentSize().width , anopponent->getContentSize().height/2));

			++elm_seq;
		}
	}

	//draw lines of tower
	for (int i = 0; i < elm_positions.size() - 1; i++) {
		auto line = DrawNode::create();
		line->drawLine(elm_positions.at(i), elm_positions.at(i + 1), Color4F(fontcolor));
		line->setLineWidth(4);
		this->addChild(line);
	}

	//draw right hand side. (opponent details and start button.)
	drawOpponentDetail(opponent,current_difficulty,elm_positions.back().y,elm_positions.front().y);


}

PlayerAI * GameStartReview::createOpponent()
{
	PlayerAI *opponent;
	int cursor = 0;
	DIFFICULTY dct_array[] = { DIFFICULTY::EASY , DIFFICULTY::NORMAL , DIFFICULTY::HARD };
	for (int i = 0; i < 3; i++) {// 3 means dct_array size
		DIFFICULTY dct = dct_array[i];
		auto names = towerMap->at(dct);
		int oppI = towerIndex - cursor;
		if (names->size() > oppI) {
			//opponent found
			opponent = new PlayerAI(names->at(oppI));
			return opponent;
		}
		else {
			//step up difficulty
			cursor += names->size();
		}
	}
	return nullptr;
}

void GameStartReview::drawOpponentDetail(Player* opponent,DIFFICULTY d,float y_low,float y_high)
{
	std::string revbg_imagedir;
	Color3B color;

	switch (d)
	{
	case DIFFICULTY::EASY:
		revbg_imagedir = "default/bg/reviewbg_easy.png";
		color = Color3B::BLACK;
		break;
	case DIFFICULTY::NORMAL:
		revbg_imagedir = "default/bg/reviewbg_normal.png";
		color = Color3B::BLACK;
		break;
	case DIFFICULTY::HARD:
		revbg_imagedir = "default/bg/reviewbg_hard.png";
		color = Color3B::WHITE;
		break;
	case DIFFICULTY::HUMAN:
		revbg_imagedir = "default/bg/reviewbg_human.png";
		color = Color3B::WHITE;
		break;
	}

	//big sprite which contains avatar, username and level info.
	Sprite *review_top_bg = Sprite::create(revbg_imagedir.c_str());
	review_top_bg->setAnchorPoint(Vec2(0.5f,0.5f));
	this->addChild(review_top_bg,999);
	review_top_bg->setPosition(Vec2(visibleSize.width*0.65f + origin.x , y_low));

	//reviewbg child elements
	Label *uname = Label::createWithTTF(opponent->get_username().c_str(), "fonts/OpenSans-ExtraBold.ttf", 20);
	uname->setColor(color);
	review_top_bg->addChild(uname);
	uname->setPosition(Vec2(review_top_bg->getContentSize().width/2,review_top_bg->getContentSize().height/2));

	ui::Layout *user_details_layout = ui::Layout::create();
	user_details_layout->setSize(Size(review_top_bg->getContentSize().width/2,0));
	user_details_layout->setLayoutType(ui::Layout::Type::VERTICAL);

	//horizontal layout below username
	ui::Layout *user_details_image_layout = ui::Layout::create();
	user_details_image_layout->setLayoutType(ui::Layout::Type::HORIZONTAL);
	user_details_image_layout->addChild(ui::ImageView::create("default/icon/level_icons/1.png"));
	user_details_image_layout->addChild(user_details_layout);
	user_details_image_layout->setContentSize(Size(review_top_bg->getContentSize().width/2,0));
	user_details_image_layout->setAnchorPoint(Vec2(0.5f,1.0f));
	review_top_bg->addChild(user_details_image_layout);
	user_details_image_layout->setPosition(Vec2(review_top_bg->getContentSize().width/2,0));

	//Create user detail labels
	char str_lt[40];

	sprintf(str_lt, "Level : %s", toString(opponent->get_level()).c_str());
	ui::Text *Level = ui::Text::create(str_lt, "fonts/OpenSans-Light.ttf", 30);
	user_details_layout->addChild(Level);

	sprintf(str_lt, "Win : %s", toString(opponent->get_win_count()).c_str());
	ui::Text *Win = ui::Text::create(str_lt,"fonts/OpenSans-Light.ttf",30);
	user_details_layout->addChild(Win);

	sprintf(str_lt, "Lost : %s", toString(opponent->get_lost_count()).c_str());
	ui::Text *Lost = ui::Text::create(str_lt, "fonts/OpenSans-Light.ttf", 30);
	user_details_layout->addChild(Lost);

	ui::Button *Play = ui::Button::create("default/btn/btnstart.png","default/btn/btnstartc.png");
	Play->setTitleText("Start Game");
	Play->setTitleFontName("fonts/OpenSans-ExtraBold.ttf");
	Play->setTitleFontSize(30);
	user_details_layout->addChild(Play);

	Play->addTouchEventListener(CC_CALLBACK_2(GameStartReview::TouchEventPlay,this));
}

/*
	RANKED GAME!
*/
void GameStartReview::drawRankedScene()
{
}
