#include "PlayerOne.h"

PlayerOne *PlayerOne::instance;
PlayerOne::PlayerOne() : Player(){
	iflags.inventory = false;
	iflags.player = false;
}

std::vector<CardList::oneCard>* PlayerOne::get_sh_ref()
{
	return &this->sh;
}

PlayerOne * PlayerOne::getInstance()
{
	if (!instance)
		instance = new PlayerOne();
	return instance;
}

bool PlayerOne::init_local()
{
	if (iflags.player) {
		cocos2d::log("YOURE TRYING INITIALIZE PLAYERONE MULTIPLE TIMES!");
		return false;
	}
	//fill in data form local.
	this->init_data(0,"USER000",0,0,1,0,0,0,0);
	this->iflags.player = true;

	cocos2d::log("%s",__func__);

	/*
	//NORMALLY IF THIS FUNC HAS BEEN CALLED, THERE'D BE NO INTERNET CONNECTION.
	//SO WE CANT DO INVENTORY OPERATIONS.
	for (int i = 0; i < this->slots; i++) {
		Card *es = (Card *)Card::create();
		es->setName("empty");
		inventory.push_back(es);
	}
	//REMOVE ABOVE WHEN ALPHA FINISHED.
	*/

	return true;
}

void PlayerOne::init_data(
	unsigned int uid,
	const char * uname,
	unsigned int tp,
	unsigned int xp,
	unsigned int level,
	unsigned int win_count,
	unsigned int lost_count,
	unsigned int pts,
	int slots
	)
{
	if (iflags.player) {
		cocos2d::log("YOURE TRYING INITIALIZE PLAYERONE MULTIPLE TIMES!");
		return;
	}
	this->uid = uid;
	this->username = uname;
	this->tp = 100000;//tp;
	this->xp = xp;
	this->level = level;
	this->lost_count = lost_count;
	this->win_count = win_count;
	this->pts = pts;
	this->slots = slots;
	this->iflags.player = true;

	cocos2d::log("%s", __func__);

	CardList::oneCard *oc;

	//init inventory with empty slots at start. 
	for (int i = 0; i < this->slots; i++) {
		oc = new CardList::oneCard;
		oc->name = "empty";
		inventory.push_back(*oc);
	}

	//init sh with empty slots
	for (int i = 0; i < 5; i++) {
		oc = new CardList::oneCard;
		oc->card_id = 0;
		oc->color = -1;
		sh.push_back(*oc);
	}
}

void PlayerOne::inventory_init(std::vector<CardList::oneCard> *inv) {
	if (inv == nullptr || iflags.inventory) {
		cocos2d::log("inventory initialization failed. func: %s",__func__);
		return;
	}
	this->inventory = *inv;
	iflags.inventory = true;
}

std::vector<CardList::oneCard> *PlayerOne::inventory_get_vector_copy(){
	std::vector<CardList::oneCard> *vec = new std::vector<CardList::oneCard>();
	std::vector<CardList::oneCard>::iterator it = inventory.begin();
	
	while (it != inventory.end()) {
		vec->push_back(*it);
		it++;
	}
	return vec;
}

int PlayerOne::inventory_add_item(Card *item){
	if (!item)	return -2;
	if (this->slots - inventory.size() < 0) return 0;
	//db stuff
	if (false)	return -1;

	CardList::oneCard *item_data = new CardList::oneCard;
	item->clone_cardinfo(item_data);
	std::vector<CardList::oneCard>::iterator card = inventory.begin();
	
	while (card != inventory.end()) {
		cocos2d::log(" func : %s ==> %s",__func__, (*card).name.c_str());
		if (strcmp((*card).name.c_str(),"empty") == 0) {
			inventory.erase(card);
			inventory.emplace(card, *item_data);
			cocos2d::log("card with name \"%s\" added to inventory.",item->get_data()->name.c_str());
			return 1;
		}
		card++;
	}
}

bool PlayerOne::inventory_delete_item() {  
	//#TODO
	cocos2d::log("%s not implemented yet.",__func__);
	return true;
}

int PlayerOne::get_empty_slots_count()
{
	std::vector<CardList::oneCard>::iterator it = inventory.begin();
	int full_slots = 0;

	while (it != inventory.end()) {
		if (strcmp((*it).name.c_str(), "empty") != 0) {
			full_slots++;
		}
		it++;
	}

	return slots - full_slots;
}

bool PlayerOne::checkout_pack(CARDPACK type, int price)
{
	if (tp >= price)
		this->tp -= price;
	else
		return false;

	//add to inventory. write to db etc.. #TODO

	switch (type)
	{
	case CARDPACK::SPECIAL: {

	}break;
	case CARDPACK::HYPER:
		break;
	case CARDPACK::MAIN:
		break;
	case CARDPACK::SUB:
		break;
	default:
		cocos2d::log("undefined tournami cardpack type. returning false. func : %s", __func__);
		return false;
	}
	return true;
}

bool PlayerOne::sell_card(Card * card)
{
	bool rmv = this->inventory_delete_item();
	if (!rmv) {
		cocos2d::log("#error : item that you want to sell couldnt removed. func : %s()",__func__);
		return false;
	}

	unsigned int card_price = static_cast<unsigned int>(card->get_price());
	cocos2d::log("TP = %u",this->tp);
	cocos2d::log("CP = %u", card_price);
	this->tp += card_price; 
	return true;
}