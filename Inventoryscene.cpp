#include "Inventoryscene.h"

USING_NS_CC;
using namespace std;
using namespace ui;


Scene* Inventoryscene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = Inventoryscene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool Inventoryscene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	this->visibleSize = Director::getInstance()->getVisibleSize();
	this->origin = Director::getInstance()->getVisibleOrigin();

	LayerColor::initWithColor(Color4B(Color3B(15, 15, 15)));

	this->player = PlayerOne::getInstance();

	/*Texture2D::TexParams bgpTexp = { GL_REPEAT,GL_REPEAT,GL_REPEAT,GL_REPEAT };
	auto bgImg = Sprite::create("default/bg/invbg.jpg", Rect(0, 0, visibleSize.width, visibleSize.height));
	bgImg->getTexture()->setTexParameters(&bgpTexp);
	this->addChild(bgImg);
	bgImg->setPosition(Vec2(visibleSize.width/2 + origin.x , visibleSize.height/2 + origin.y));*/

	//set touchlistener.
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(Inventoryscene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(Inventoryscene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(Inventoryscene::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


	//return to main menu button
	auto home_btn = Button::create("default/btn/home.png", "default/btn/home.png");
	this->addChild(home_btn);
	home_btn->setPosition(Vec2(visibleSize.width + origin.x - (home_btn->getContentSize().width*0.8f), visibleSize.height + origin.y - (home_btn->getContentSize().height*0.8f)));

	home_btn->addTouchEventListener([&, this](Ref* sender, Widget::TouchEventType type) {
		switch (type) {
		case cocos2d::ui::Widget::TouchEventType::ENDED:
			Director::getInstance()->popScene();
			break;
		}
	});
		
	//empty slot instance
	this->empty_slot_instance = Sprite::create("default/assets/c/empty-slot.png");
	this->empty_slot_instance->setName("empty");
	//also will be reference for current card size.
	this->card_size = this->empty_slot_instance->getContentSize();

	//header text
	auto header = Label::createWithTTF("INVENTORY","fonts/OpenSans-ExtraBold.ttf",40);
	header->setColor(Color3B::WHITE);
	this->addChild(header);
	header->setPosition(Vec2(visibleSize.width/2 + origin.x , visibleSize.height - header->getContentSize().height));
	
	/*auto inventory_icon = Sprite::create("default/icon/inventroy.png");
	this->addChild(inventory_icon);
	inventory_icon->setPosition(Vec2(header->getBoundingBox().getMinX() - inventory_icon->getContentSize().width/2 + origin.x , header->getPosition().y));
	*/
	//coin o'meter
	auto coin_layer = LayerColor::create(Color4B(Color3B(242, 278, 71)));
	auto coin_layer_icon = Sprite::create("default/icon/coin.png");
	coin_label = Label::createWithTTF(player->toString(player->get_tp()), "fonts/OpenSans-Light.ttf", 30);
	coin_label->setColor(Color3B::WHITE);
	coin_layer->setContentSize(Size(250, coin_layer_icon->getContentSize().height));
	this->addChild(coin_layer, 5);
	coin_layer->setPosition(Vec2(coin_layer->getContentSize().width / 2, header->getPosition().y - coin_layer->getContentSize().height/2));
	coin_layer->addChild(coin_layer_icon);
	coin_layer->addChild(coin_label);
	coin_layer_icon->setAnchorPoint(Vec2(0, 0.5f));
	coin_layer_icon->setPosition(Vec2(0, coin_layer->getContentSize().height / 2));
	coin_label->setAnchorPoint(Vec2(1, 0.5f));
	coin_label->setPosition(Vec2(coin_layer->getContentSize().width - 5, coin_layer->getContentSize().height / 2));

	this->prepare_starting_hand_visuals();
	this->load_inventory();
	this->prepare_scroller();
	this->set_cards_listeners();
	this->prepare_op_buttons();
	this->cardMenu_prepare();

	return true;
}

bool Inventoryscene::onTouchBegan(Touch * touch, Event * event)
{
	return true;
}

void Inventoryscene::onTouchMoved(Touch * touch, Event * event)
{
}

void Inventoryscene::onTouchEnded(Touch * touch, Event * event)
{
	Vec2 p = touch->getLocation();
	Rect rc;
	std::vector<Sprite *>::iterator sh = this->starting_hand.begin();
	while (sh != this->starting_hand.end()) {
		rc = (*sh)->getBoundingBox();
		if (rc.containsPoint(p)) {
			this->selected_item = *sh;
			cardMenu_pop(MenuType::CARD_MENU_SH);
			//this->selected_item = nullptr;
			//if assigned nullptr to selected_item here, it wont add to sh.
			return;
		}
		sh++;
	}

	//burda butun inventoryi taramaya gerek yok. sadece ekrandakileri tarat.
	std::vector<Sprite *>::iterator it = this->inventory.begin();
	while (it != this->inventory.end()) {
		rc = (*it)->getBoundingBox();
		if (rc.containsPoint(p)) {
			Item *target = dynamic_cast<Item *>(*it);
			if (!target)	return;
			this->selected_item = *it;
			if (target->get_type() == ITEMTYPE::ITEM_CARD) {
				cardMenu_pop(MenuType::CARD_MENU_INVENTORY);
			}
			else if (target->get_type() == ITEMTYPE::ITEM_PACK) {
				cardMenu_pop(MenuType::CARD_MENU_PACK);
			}
			//this->selected_item = nullptr;
			//if assigned nullptr to selected_item here, it wont add to sh.
			return;
		}
		it++;
	}
	cardMenu_hide();
	this->selected_item = nullptr;
	//not touched card variants.
}

void Inventoryscene::cardMenu_prepare() {
	auto btn_add_tosh = Button::create("default/btn/card_menu_button_add.png");
	this->addChild(btn_add_tosh, 9999);
	btn_add_tosh->addTouchEventListener([&](cocos2d::Ref *sender, ui::Widget::TouchEventType type) {
		switch (type) {
			case ui::Widget::TouchEventType::ENDED: {
				this->cardmenu_add();
			}break;
		}
	});

	auto btn_remove = Button::create("default/btn/card_menu_button_remove.png");
	this->addChild(btn_remove, 9999);
	btn_remove->addTouchEventListener([&](cocos2d::Ref *sender, ui::Widget::TouchEventType type) {
		switch (type) {
		case ui::Widget::TouchEventType::ENDED: {
			this->cardmenu_remove();
		}break;
		}
	});

	auto btn_sell = Button::create("default/btn/card_menu_button_sell.png");
	this->addChild(btn_sell, 9999);
	btn_sell->addTouchEventListener([&](cocos2d::Ref *sender, ui::Widget::TouchEventType type) {
		switch (type) {
		case ui::Widget::TouchEventType::ENDED: {
			this->cardmenu_sell();
		}break;
		}
	});

	auto btn_openpack = Button::create("default/btn/card_menu_button_open.png");
	this->addChild(btn_openpack, 9999);
	btn_openpack->addTouchEventListener([&](cocos2d::Ref *sender, ui::Widget::TouchEventType type) {
		switch (type) {
		case ui::Widget::TouchEventType::ENDED: {
			this->cardmenu_openthepack();
		}break;
		}
	});


	std::map<int, Button *>::iterator it = this->cardmenu.begin();
	this->cardmenu.insert(it,std::pair<int,Button *>(0,btn_add_tosh));
	this->cardmenu.insert(it,std::pair<int, Button *>(1, btn_openpack));
	this->cardmenu.insert(it,std::pair<int, Button *>(2, btn_sell));
	this->cardmenu.insert(it,std::pair<int, Button *>(3, btn_remove));
	cardMenu_hide();

}

void Inventoryscene::cardMenu_pop(MenuType type) {
	//place buttons upon card.
	if (this->selected_item != nullptr) {
		cocos2d::log("cardmenu z order : %.2f",this->cardmenu[0]->getZOrder());
		float h = this->cardmenu[0]->getContentSize().height + 10;
		Vec2 p1 = Vec2(this->selected_item->getPosition().x + origin.x, this->selected_item->getPosition().y + (h / 2) + origin.y );
		Vec2 p2 = Vec2(this->selected_item->getPosition().x + origin.x, this->selected_item->getPosition().y - (h / 2) + origin.y);

		switch (type) {
			case MenuType::CARD_MENU_INVENTORY: {
				if (this->selected_item->getName() == "empty") {
					this->selected_item = nullptr;
					cardMenu_hide();
					return;
				}
				this->cardmenu[0]->setVisible(true);
				this->cardmenu[0]->setPosition(p1);
				this->cardmenu[2]->setVisible(true);
				this->cardmenu[2]->setPosition(p2);
				this->cardmenu[1]->setVisible(false);
				this->cardmenu[3]->setVisible(false);
			}break;
			case MenuType::CARD_MENU_SH: {
				if (this->selected_item->getName() == "empty") {
					this->selected_item = nullptr;
					cardMenu_hide();
					return;
				}
				this->cardmenu[3]->setVisible(true);
				this->cardmenu[3]->setPosition(p1);
				this->cardmenu[0]->setVisible(false);
				this->cardmenu[1]->setVisible(false);
				this->cardmenu[2]->setVisible(false);
			}break;
			case MenuType::CARD_MENU_PACK: {
				this->cardmenu[1]->setVisible(true);
				this->cardmenu[1]->setPosition(p1);
				this->cardmenu[0]->setVisible(false);
				this->cardmenu[2]->setVisible(false);
				this->cardmenu[3]->setVisible(false);
			}break;
		}
		//pack menu here!.
	}
}

void Inventoryscene::cardMenu_hide() {
	for (int i = 0; i < this->cardmenu.size(); i++) {
		this->cardmenu[i]->setVisible(false);
	}
}

void Inventoryscene::cardmenu_add()
{
	if (this->selected_item != nullptr) {
		std::vector<Sprite *> *vec = &this->starting_hand;
		std::vector<Sprite *>::iterator slot = vec->begin();
		Card *c2 = dynamic_cast<Card *>(this->selected_item);//delegate for selected_item
		if (!c2)	return;
		while (slot != vec->end()) {
			if ((*slot)->getName() != "empty") {
				Card *c1 = dynamic_cast<Card *>(*slot);
				if (*c1==*c2) {
					return;
				}
			}
			slot++;
		}
		slot = vec->begin();
		while (slot != vec->end()) {
			if ((*slot)->getName() == "empty") {
				//move visuals to places
				Vec2 p = this->selected_item->getPosition();
				CardList::oneCard new_card;
				c2->clone_cardinfo(&new_card);//delegate for selected_item
				Sprite *new_slot = Card::create(&new_card,true);
				this->addChild(new_slot,max_z_order);
				new_slot->setPosition((*slot)->getPosition());
				//delete empty from sh then add the selected card
				Sprite *s = *slot;
				vec->erase(slot);
				vec->emplace(slot,new_slot);
				s->removeFromParentAndCleanup(true);
				this->selected_item = nullptr;
				//hide little menu
				cardMenu_hide();
				return;
			}
			slot++;
		}
	}
	cardMenu_hide();
}


void Inventoryscene::cardmenu_remove() {
	if (selected_item != nullptr) {
		std::vector<Sprite *> *vec = &starting_hand;
		std::vector<Sprite *>::iterator sh_slot = vec->begin();
		Card *remove_this_card = dynamic_cast<Card *>(selected_item);
		while (sh_slot != vec->end()) {
			Card *slot_card = dynamic_cast<Card *>(*sh_slot);
				
			if (slot_card && slot_card->getName() != "empty" && *remove_this_card == *slot_card) {
				Vec2 p = remove_this_card->getPosition();
				auto empty = Sprite::create("default/assets/c/empty-slot.png");
				empty->setName("empty");
				this->addChild(empty,max_z_order);
				empty->setPosition(p);
				vec->erase(sh_slot);
				vec->emplace(sh_slot,empty);
				remove_this_card->removeFromParentAndCleanup(true);
				this->selected_item = nullptr;
				cardMenu_hide();
				return;
			}
			sh_slot++;
		}
	}
	cardMenu_hide();
}

void Inventoryscene::cardmenu_openthepack() {
	if (this->selected_item == nullptr) {
		cardMenu_hide();
		return;
	}
	CardPack *cardpack = dynamic_cast<CardPack *>(this->selected_item);

	if (!cardpack) {
		this->selected_item = nullptr;
		cardMenu_hide();
		return;
	}

	if (cardpack->get_type() != ITEMTYPE::ITEM_PACK) {
		this->selected_item = nullptr;
		cardMenu_hide();
		return;
	}

	this->selected_item = nullptr;
	cardMenu_hide();
	return;
}

void Inventoryscene::cardmenu_sell()
{
	if (selected_item == nullptr) return;
	Card *card = dynamic_cast<Card *>(selected_item);

	if (!player->sell_card(card)) {
		//ADD MBOX HERE!!
		cocos2d::log("error on sell operation.");
		return;
	}

	ostringstream stm;
	stm << player->get_tp();
	coin_label->setString(stm.str().c_str());
	cocos2d::log("player new tp : %u",player->get_tp());
}

void Inventoryscene::load_inventory()
{
	CardList::oneCard data;
	data.avg = 30;
	data.color = CARDCOLOR_SILVER;
	data.land = "POR";
	data.name = "CET";
	data.position = "FW";
	data.rate = 2;
	data.team = "BAR";

	std::vector<CardList::oneCard> *player_inventory_vector = player->inventory_get_vector_copy();
	std::vector<CardList::oneCard>::iterator oc_it = player_inventory_vector->begin();

	while(oc_it != player_inventory_vector->end()) {
		Sprite *lbl;
		
		if ((*oc_it).name == "empty") {
			lbl = Card::create();
		}
		else {
			lbl = Card::create(&(*oc_it),true);
		}
		lbl->setAnchorPoint(Vec2(0.5f, 0.5f));
		this->inventory.push_back(lbl);
		oc_it++;
	}
	//set starting hand data.
}

void Inventoryscene::prepare_scroller()
{
	this->scroll_layer = LayerColor::create(Color4B(Color3B(50,50,50)));
	this->scroll_layer->setContentSize(Size(visibleSize.width + origin.x, card_size.height + 10));
	this->addChild(this->scroll_layer,max_z_order);
	max_z_order++;
	this->scroll_layer->setPosition(Vec2(origin.x, origin.y));

	this->chv_right = Button::create("default/btn/chevyron-r.png");
	this->chv_right->setAnchorPoint(Vec2(1, 0));
	this->chv_right->setPosition(Vec2(visibleSize.width - this->chv_right->getContentSize().width,0));
	scroll_layer->addChild(this->chv_right);

	this->chv_left = Button::create("default/btn/chevyron-l.png");
	this->chv_left->setAnchorPoint(Vec2(0, 0));
	this->chv_left->setPosition(Vec2(this->chv_left->getContentSize().width,0));
	scroll_layer->addChild(this->chv_left);

	float card_area_size_x = this->chv_right->getBoundingBox().getMinX() - this->chv_left->getBoundingBox().getMaxX();
	this->card_margin_h = card_size.width / 6;
	this->card_count_on_screen = floor(card_area_size_x / (card_size.width + this->card_margin_h));
	this->card_count_on_screen -= 1; //this is for "add extra slot" button.
	cocos2d::log("card count : %d", card_count_on_screen);

	this->first_card_position_on_scroller = Vec2(card_margin_h + chv_left->getBoundingBox().getMaxX() + card_size.width/2,scroll_layer->getContentSize().height/2);
	this->shift_x = card_size.width + card_margin_h;
	this->last_card_position_on_scroller = Vec2(this->first_card_position_on_scroller.x + ((card_count_on_screen-1)*shift_x), scroll_layer->getContentSize().height / 2);
}

void Inventoryscene::set_cards_listeners()
{
	std::vector<Sprite *>::iterator it = inventory.begin();
	
	for (int i = 0; i < inventory.size() && it != inventory.end(); i++, it++) {
		this->scroll_layer->addChild(*it);
		if(i < card_count_on_screen)
			(*it)->setPosition(this->first_card_position_on_scroller.x + (i*shift_x),this->first_card_position_on_scroller.y);
		else
			(*it)->setPosition(this->last_card_position_on_scroller.x, -card_size.height);
	}

	auto lbl = Button::create("default/btn/add_slots.png");
	lbl->setAnchorPoint(Vec2(0.5f, 0.5f));
	scroll_layer->addChild(lbl);
	lbl->setPosition(Vec2(this->last_card_position_on_scroller.x + shift_x,this->last_card_position_on_scroller.y));
	auto scaleup = ScaleTo::create(0.8f, 1.1f);
	auto scaledown = ScaleTo::create(0.8f, 1.0f);
	lbl->runAction(RepeatForever::create(Sequence::create(scaleup,scaledown,nullptr)));

	this->chv_right->addTouchEventListener([&,this](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
		switch (type) {
			case cocos2d::ui::Widget::TouchEventType::BEGAN: {
				this->scroll_right();
			}break;
		}
	});
	this->chv_left->addTouchEventListener([&, this](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
		switch (type) {
			case cocos2d::ui::Widget::TouchEventType::BEGAN: {
				this->scroll_left();
			}break;
		}
	});
}

bool Inventoryscene::scroll_right()
{
	if (inventory.size() <= card_count_on_screen) {
		return false;
	}
	if (scroll_factor_i == (inventory.size() - card_count_on_screen)) {
		return false;
	}
	else {
		auto first = inventory.at(scroll_factor_i);
		first->setPosition(this->first_card_position_on_scroller.x, -card_size.height);
		auto new_one = inventory.at(scroll_factor_i + card_count_on_screen);
		auto movetonew = MoveTo::create(0.2f, this->last_card_position_on_scroller);
		auto movetoleft = MoveBy::create(0.2f,Vec2(-shift_x,0));
		
		for (int i = scroll_factor_i; i < scroll_factor_i + card_count_on_screen; i++) {
			first = inventory.at(i);
			first->runAction(movetoleft->clone());
		}
		new_one->runAction(movetonew);
		scroll_factor_i++;
		cardMenu_hide();
		return true;
	}
}

bool Inventoryscene::scroll_left()
{
	if (inventory.size() <= card_count_on_screen) {
		return false;
	}
	if (scroll_factor_i == 0) {
		return false;
	}
	else {
		auto last = inventory.at(scroll_factor_i + card_count_on_screen -1);
		last->setPosition(this->last_card_position_on_scroller.x, -card_size.height);
		auto new_one = inventory.at(scroll_factor_i - 1);
		auto movetonew = MoveTo::create(0.2f, this->first_card_position_on_scroller);
		auto movetoright = MoveBy::create(0.2f, Vec2(shift_x, 0));

		for (int i = scroll_factor_i + card_count_on_screen -1; i >= scroll_factor_i; i--) {
			last = inventory.at(i);
			last->runAction(movetoright->clone());
		}
		new_one->runAction(movetonew);
		scroll_factor_i--;
		cardMenu_hide();
		return true;
	}
}

void Inventoryscene::prepare_starting_hand_visuals()
{
	auto bg = Sprite::create("default/bg/sh_cover.png");
	bg->setPosition(Vec2(visibleSize.width/2 + origin.x , visibleSize.height/2 + origin.y));
	this->addChild(bg,max_z_order);
	max_z_order++;
	Vec2 pmid = Vec2(visibleSize.width/2 + origin.x,visibleSize.height/2 + origin.y);
	LocalDb *db = new LocalDb();
	db->get_player_sh();
	std::vector<CardList::oneCard> *sh = player->get_sh_ref();
	std::vector<CardList::oneCard>::iterator sh_elm = sh->begin();

	while (sh_elm != sh->end()) {
		CardList::oneCard &oc = *sh_elm;
		if (oc.card_id == 0) {
			auto empty_sh_slot = Sprite::create("default/assets/c/empty-slot.png");
			empty_sh_slot->setName("empty");
			this->addChild(empty_sh_slot, max_z_order);
			this->starting_hand.push_back(empty_sh_slot);
		}else{
			Card *sh_card = (Card *)Card::create(&oc,true);
			this->addChild(sh_card,max_z_order);
			this->starting_hand.push_back(sh_card);
		}
		sh_elm++;
	}
	max_z_order++;

	float mx = this->starting_hand.at(0)->getContentSize().width + 10;
	this->starting_hand.at(0)->setPosition(Vec2(pmid.x - 2*mx, pmid.y));
	this->starting_hand.at(1)->setPosition(Vec2(pmid.x - 1*mx, pmid.y));
	this->starting_hand.at(2)->setPosition(Vec2(pmid.x , pmid.y));
	this->starting_hand.at(3)->setPosition(Vec2(pmid.x + 1*mx, pmid.y));
	this->starting_hand.at(4)->setPosition(Vec2(pmid.x + 2*mx, pmid.y));

	this->save_sh_button = Button::create("default/btn/card_menu_button_save.png", "default/btn/card_menu_button_save_c.png");
	this->addChild(this->save_sh_button, max_z_order);
	this->save_sh_button->addTouchEventListener([&](cocos2d::Ref *sender, ui::Widget::TouchEventType type) {
		switch (type) {
		case ui::Widget::TouchEventType::ENDED: {
			//NO NEED TO CALL SH_REF AND OVERRÝDE ÝT.
			//just create new sh vector with changes occured.
			//then save it through localdb class.
			//and when inventory scene starts call localdb's get_player_sh() method.
			//it fills up the actual sh vector with complete oneCard values.
			//then we will be able to create cards here to prepare visuals.

			cocos2d::log("save clicked");
			std::vector<Sprite *>::iterator sh_item = this->starting_hand.begin();
			std::vector<CardList::oneCard> sh_save_vec;

			while (sh_item != this->starting_hand.end()) {
				CardList::oneCard *oc = new CardList::oneCard;
				if ((*sh_item)->getName() == "empty") {
					oc->card_id = 0;
					oc->color = -1;
				}
				else {
					Card *sh_card = dynamic_cast<Card *>(*sh_item);
					oc->card_id = sh_card->get_id();
					oc->color = sh_card->get_data()->color;
				}
				sh_save_vec.push_back(*oc);
				sh_item++;
			}
			LocalDb *db = new LocalDb();
			if (!db->save_player_sh(&sh_save_vec)) {
				cocos2d::log("error during save starting hand.");
			}
			db->get_player_sh();
		}break;
		}
	});
	this->save_sh_button->setAnchorPoint(Vec2(1.0f,0));
	this->save_sh_button->setPosition(Vec2(bg->getBoundingBox().getMaxX() + origin.x,bg->getBoundingBox().getMaxY()+origin.y + 10));
}

void Inventoryscene::prepare_op_buttons()
{
	int margin_left = 10;
}
