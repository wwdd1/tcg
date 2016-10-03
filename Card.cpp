#include "Card.h"

USING_NS_CC;

Sprite * Card::create()
{
	const char *unique_id = "empty";
	Card *sprite = new (std::nothrow) Card(0,unique_id,0,ITEMTYPE::ITEM_CARD);
	if (sprite->init() && sprite) {
		sprite->autorelease();
		sprite->setName("empty");
		std::string dst = CARDS_DIR;
		dst += CARD_EMPTY;
		Sprite *ec = Sprite::create(dst.c_str());
		sprite->addChild(ec);
		sprite->setContentSize(ec->getContentSize());
		ec->setPosition(Vec2(ec->getContentSize().width/2,ec->getContentSize().height/2));
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

Sprite *Card::create(CardList::oneCard * data, bool isfrontside) {
	Card *sprite = new (std::nothrow) Card(data->card_id, "UID01", data->price, ITEMTYPE::ITEM_CARD);
	if (sprite && sprite->init())
	{
		sprite->autorelease();
		sprite->isFS = isfrontside;
		sprite->setContentSize(Card::getDefinedContentSize());
		sprite->setAnchorPoint(Vec2(0.5f, 0.5f));
		(data == nullptr) ? sprite->createEmptyCard() : sprite->attachElements(data);
		sprite->set_card_data(data);
		//if (data != nullptr)
			//sprite->addEvents();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

cocos2d::Size Card::getDefinedContentSize()
{
	//WHEN CONSTNT VALUES DELIVERED IN A SIMPLE ONE FILE , OPTIMISE THIS FUNCTION!!!!
	string dir = CARDS_DIR;
	auto delegate_size_sprite = Sprite::create(dir + CARD_BG);
	return (cocos2d::Size)(delegate_size_sprite->getContentSize());
}

void Card::attachElements(CardList::oneCard *data) {
	Color3B colors_on_card;
	if (data->color == CARDCOLOR_BLUE || data->color == CARDCOLOR_GREEN) {
		colors_on_card = Color3B::WHITE;
	}
	else {
		colors_on_card = Color3B::BLACK;
	}

	std::string dir = CARDS_DIR;
	auto *bg = Sprite::create(dir+CARD_BG);
	bg->setAnchorPoint(Vec2(0.5f,0.5f));
	bg->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));

	std::ostringstream stm;
	stm << data->color;
	auto *fg = Sprite::create(dir+stm.str()+".png");
	fg->setAnchorPoint(Vec2(0.5f,0.5f));
	fg->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));

	this->addChild(bg, 0, "bg");
	this->addChild(fg, 1, "fg");

	std::ostringstream st;
	st << data->avg;
	auto avg = Label::createWithTTF(st.str(), "fonts/OpenSans-ExtraBoldItalic.ttf", 35*0.75);
	
	avg->setColor(colors_on_card);
	avg->setAnchorPoint(Vec2(0,0.5f));
	avg->setPosition(Vec2(avg->getContentSize().width/8,fg->getContentSize().height/2));
	fg->addChild(avg, 0);
	
	dir = TEAMS_DIR;
	auto team = Sprite::create(dir + data->team + ".png");
	team->setAnchorPoint(Vec2(1.0f,0.5f));
	team->setPosition(Vec2(fg->getContentSize().width - team->getContentSize().width/8,fg->getContentSize().height/2));
	fg->addChild(team, 1);

	auto name = Label::createWithTTF(data->name, "fonts/OpenSans-Semibold.ttf",15 * 0.75);
	name->setColor(colors_on_card);
	name->setAnchorPoint(Vec2(0.5f,0));
	name->setPosition(Vec2(this->getContentSize().width/2 , name->getContentSize().height*0.4));
	fg->addChild(name,3);

	dir = POSITIONS_DIR;
	auto position = Sprite::create(dir + data->position + ".png");
	position->setAnchorPoint(Vec2(0,1.0f));
	position->setPosition(Vec2(position->getContentSize().width/10,fg->getContentSize().height - position->getContentSize().height/8));
	fg->addChild(position);

	dir = LANDS_DIR;
	auto futils = FileUtils::getInstance();
	std::string nat_dir = dir + data->land + ".png";
	cocos2d::Data nat_flag_file = futils->getDataFromFile(nat_dir);
	if (!nat_flag_file.isNull()) {
		auto land = Sprite::create(nat_dir);
		land->setAnchorPoint(Vec2(0, 1.0f));
		land->setPosition(Vec2(position->getPositionX() + position->getContentSize().width, position->getPositionY()));
		fg->addChild(land, 2);
	}

	if (!this->isFrontside()) {
		this->flipToBackside(true);
	}
}

void Card::createEmptyCard() {
	std::string dir = CARDS_DIR;
	auto *bg = Sprite::create(dir + CARD_BG);
	bg->setAnchorPoint(Vec2(0.5f, 0.5f));
	bg->setPosition(Vec2(this->getContentSize().width/2,this->getContentSize().height/2));
	this->addChild(bg);
}

void Card::set_card_data(CardList::oneCard * data)
{
	if (data == nullptr)	return;//its just bg image card.
	this->c = new struct card_data;
	c->card_id = data->card_id;
	c->avg = data->avg;
	c->color = data->color;
	c->land = data->land;
	c->name = data->name;
	c->position = data->position;
	c->rate = data->rate;
	c->team = data->team;
}

bool Card::flipToBackside() {
	try {
		if (!isFrontside())	return true;
		auto skw = RotateBy::create(0.1f, Vec3(0.0f, 90.0f, 0.0f));
		auto changeFaces = CallFunc::create([this] { this->z(BACKSIDE); });
		auto sq = Sequence::create(skw, changeFaces, skw, nullptr);
		this->runAction(sq);
		this->isFS = false;
		return true;
	}
	catch (std::exception ex) {
		cocos2d::log("%s",ex.what());
		return false;
	}
}

bool Card::flipToBackside(bool first) {
	try {
		auto skw = RotateBy::create(0.1f, Vec3(0.0f, 90.0f, 0.0f));
		auto changeFaces = CallFunc::create([this] { this->z(BACKSIDE); });
		auto sq = Sequence::create(skw, changeFaces, skw, nullptr);
		this->runAction(sq);
		this->isFS = false;
		return true;
	}
	catch (std::exception ex) {
		cocos2d::log("%s", ex.what());
		return false;
	}
}

bool Card::flipToFrontside() {
	try {
		if (isFrontside())	return true;
		auto skw = RotateBy::create(0.1f, Vec3(0.0f, 90.0f, 0.0f));
		auto changeFaces = CallFunc::create([this] { this->z(FRONTSIDE); });
		auto sq = Sequence::create(skw, changeFaces, skw, nullptr);
		this->runAction(sq);
		this->isFS = true;
		return true;
	}
	catch (std::exception ex) {
		cocos2d::log("%s",ex.what());
		return false;
	}
}

void Card::z(signed int z) {
	this->reorderChild(this->getChildByName("bg"), z);
}

bool Card::isFrontside() {
	return (this->isFS);
}

void Card::setSelected(bool s)
{
	this->selected = s;
}

bool Card::isSelected()
{
	return this->selected;
}

bool Card::isbeingPlayed() {
	return (this->bengPl);
}

void Card::set_beingPlayed(bool bp) {
	this->bengPl = bp;
}

bool Card::ishandOn()
{
	return (this->is_handOn);
}

bool Card::isClicked()
{
	return this->clicked;
}

void Card::set_handOn(bool hon)
{
	this->is_handOn = hon;
}

void Card::set_clicked(bool c)
{
	this->clicked = c;
}

Card::CardData *Card::get_data()
{
	return this->c;
}

void Card::clone_cardinfo(CardList::oneCard *oc)
{
	oc->avg = this->get_data()->avg;
	oc->card_id = this->get_data()->card_id;
	oc->color = this->get_data()->color;
	oc->land = this->get_data()->land;
	oc->name = this->get_data()->name;
	oc->position = this->get_data()->position;
	oc->rate = this->get_data()->rate;
	oc->team = this->get_data()->team;
	oc->price = this->get_price();
}

void Card::addEvents()
{
	auto listener = cocos2d::EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [&](cocos2d::Touch* touch, cocos2d::Event* evt)
	    {
	        cocos2d::Vec2 p = touch->getLocation();
	        cocos2d::Rect rect = this->getBoundingBox();

	        if(rect.containsPoint(p))
	        {
				cocos2d::log("touch event fired.!");
				auto curr_card = dynamic_cast<Card *>(evt->getCurrentTarget());
				curr_card->set_clicked(true);
				return true; // to indicate that we have consumed it.
	        }

	        return false; // we did not consume this event, pass thru.
	    };

	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void Card::suspendEvents()
{
	Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(this);
}

void Card::resumeEvents()
{
	Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(this);
}
