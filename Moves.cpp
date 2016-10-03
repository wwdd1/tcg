#include "Moves.h"
Moves * Moves::_mv;	//singleton pattern requirements...
Moves::Moves(){}

Moves * Moves::getInstance()
{
	if (!_mv)
		_mv = new Moves();
	return _mv;
}

bool Moves::init(float card_width,float card_height,cocos2d::Scene *gamescene){
	this->gamescene = gamescene;
	this->card_width = card_width;
	this->card_height = card_height;

	this->vs = Director::getInstance()->getVisibleSize();
	this->origin = Director::getInstance()->getVisibleOrigin();

	//mesure player hand cards position start point x
	const float playerhand_startpoint_x = (this->vs.width - ((this->card_width*0.7f) * 4 + this->card_width)) / 2; //4 cards with space between + last card

	this->player_slot_points.insert(this->player_slot_points.begin() + 0, Vec2(playerhand_startpoint_x + (this->card_width*1.2f * 0) + this->origin.x, (this->card_height * 0.8f) + this->origin.y));
	this->player_slot_points.insert(this->player_slot_points.begin() + 1, Vec2(playerhand_startpoint_x + (this->card_width*1.2f * 1) + this->origin.x, (this->card_height * 0.8f) + this->origin.y));
	this->player_slot_points.insert(this->player_slot_points.begin() + 2, Vec2(playerhand_startpoint_x + (this->card_width*1.2f * 2) + this->origin.x, (this->card_height * 0.8f) + this->origin.y));
	this->player_slot_points.insert(this->player_slot_points.begin() + 3, Vec2(playerhand_startpoint_x + (this->card_width*1.2f * 3) + this->origin.x, (this->card_height * 0.8f) + this->origin.y));
	this->player_slot_points.insert(this->player_slot_points.begin() + 4, Vec2(playerhand_startpoint_x + (this->card_width*1.2f * 4) + this->origin.x, (this->card_height * 0.8f) + this->origin.y));
	/*PLAYER ONE CARDPOINTS MEASURED.*/

	const float player2hand_startpoint_x = (*(this->player_slot_points.begin())).x;

	this->player2_slot_points.insert(this->player2_slot_points.begin() + 0, Vec2(player2hand_startpoint_x + (this->card_width*1.2f * 0), this->vs.height + this->origin.y));
	this->player2_slot_points.insert(this->player2_slot_points.begin() + 1, Vec2(player2hand_startpoint_x + (this->card_width*1.2f * 1), this->vs.height + this->origin.y));
	this->player2_slot_points.insert(this->player2_slot_points.begin() + 2, Vec2(player2hand_startpoint_x + (this->card_width*1.2f * 2), this->vs.height + this->origin.y));
	this->player2_slot_points.insert(this->player2_slot_points.begin() + 3, Vec2(player2hand_startpoint_x + (this->card_width*1.2f * 3), this->vs.height + this->origin.y));
	this->player2_slot_points.insert(this->player2_slot_points.begin() + 4, Vec2(player2hand_startpoint_x + (this->card_width*1.2f * 4), this->vs.height + this->origin.y));
	/*PLAYER TWO CARDPOINTS MEASURED.*/

	this->mid_slot_points[0] = Vec2(this->vs.width / 2 + this->origin.x + (card_width*0.8f), this->vs.height / 2 + this->origin.y); //player 1
	this->mid_slot_points[1] = Vec2(this->vs.width / 2 + this->origin.x - (card_width*0.8f), this->vs.height / 2 + this->origin.y); //player 2
	/*MIDPOINT*/

	std::vector<cocos2d::Vec2>::iterator it;
	for (it = this->player_slot_points.begin(); it < this->player_slot_points.end(); it++) {
		cocos2d::log("player slot vectors : (%f,%f)", (*it).x, (*it).y);
	}
	cocos2d::log("card width height : %f,%f",card_width,card_height);
	return true;
}

void Moves::set_deck_position(Vec2 point) {
	this->deck_position_points = point;
}

bool Moves::deployCardsToPlayers(std::vector<cocos2d::Sprite *> *card_sprites,int which_player) {
	std::vector<cocos2d::Vec2>::iterator toPoint;
	std::vector<cocos2d::Sprite *>::iterator card = card_sprites->begin();

	if (which_player == 1) {
		toPoint = this->player_slot_points.begin();
	}
	else if (which_player == 2) {
		toPoint = this->player2_slot_points.begin();
	}

	(*card)->setPosition(this->deck_position_points);
	this->gamescene->addChild((*card));
	(*(++card))->setPosition(this->deck_position_points);
	this->gamescene->addChild((*card));
	(*(++card))->setPosition(this->deck_position_points);
	this->gamescene->addChild((*card));
	(*(++card))->setPosition(this->deck_position_points);
	this->gamescene->addChild((*card));
	(*(++card))->setPosition(this->deck_position_points);
	this->gamescene->addChild((*card));

	card = card_sprites->begin();

	auto move1 = cocos2d::MoveTo::create(0.3f, (*toPoint));
	auto move2 = cocos2d::MoveTo::create(0.3f, (*(++toPoint)));
	auto move3 = cocos2d::MoveTo::create(0.3f, (*(++toPoint)));
	auto move4 = cocos2d::MoveTo::create(0.3f, (*(++toPoint)));
	auto move5 = cocos2d::MoveTo::create(0.3f, (*(++toPoint)));

	auto sq1 = cocos2d::Sequence::create(move1, nullptr);
	(*card)->runAction(sq1);
	auto sq2 = cocos2d::Sequence::create(move2, nullptr);
	(*(++card))->runAction(sq2);
	auto sq3 = cocos2d::Sequence::create(move3, nullptr);
	(*(++card))->runAction(sq3);
	auto sq4 = cocos2d::Sequence::create(move4, nullptr);
	(*(++card))->runAction(sq4);
	auto sq5 = cocos2d::Sequence::create(move5, nullptr);
	(*(++card))->runAction(sq5);

	cocos2d::log("player%d cards deployed.",which_player);
	return true;
}

bool Moves::cardToEmptySlot(int player,int slot, cocos2d::Sprite *new_card_to_hand)
{
	std::vector<cocos2d::Vec2> slot_points_of_player;
	if (player == 1) {
		slot_points_of_player = this->player_slot_points;
	}
	else if (player == 2) {
		slot_points_of_player = this->player2_slot_points;
	}
	auto isfsctrl = CallFunc::create([new_card_to_hand,player] {if (player == 1 && !((Card *)new_card_to_hand)->isFrontside()) { ((Card *)new_card_to_hand)->flipToFrontside(); } });
	auto sq = Sequence::create(DelayTime::create(2.0f),MoveTo::create(0.3f,*(slot_points_of_player.begin()+slot)),isfsctrl,nullptr);
	new_card_to_hand->setPosition(this->deck_position_points);
	this->gamescene->addChild(new_card_to_hand);
	new_card_to_hand->runAction(sq);
	return true;
}

bool Moves::gameareaAnnounceTextSlide(cocos2d::Label *announce)
{
	announce->setAnchorPoint(Vec2(0.5f,0.5f));
	announce->setPosition(Vec2(this->origin.x + this->vs.width/2 + announce->getContentSize().width/2,this->vs.height/2 + this->origin.y));
	
	auto move = MoveTo::create(0.5f, Vec2(this->origin.x + this->vs.width/2,this->origin.y + this->vs.height/2));
	auto delay = DelayTime::create(2.0f);
	auto move2 = MoveTo::create(0.5f, Vec2(-announce->getContentSize().width-this->origin.x, this->origin.y + this->vs.height / 2));
	auto sq = Sequence::create(move,delay,move2,nullptr);
	announce->runAction(sq);
	return true;
}

bool Moves::play_PlayerOne(cocos2d::Sprite *selected_card)
{
	selected_card->runAction(MoveTo::create(0.2f, this->mid_slot_points[0]));
	TCGStatus::getInstance()->playerOnePlayed(true);//PLAYER 1 PLAYED! TURN OF PLAYER 2
	StageData::getInstance()->addToMiddleStack(selected_card);
	//((Gamescene *)(this->gamescene))->start_game_loop(selected_card); //routes to master gameloop inside actual gamelayer
	/*we came from card's touchlistener func. here. so the statement below should be here. not in Gamescene!.*/
	return true;
}

bool Moves::play_PlayerTwo(cocos2d::Sprite *selected_card)
{
	((Card *)selected_card)->set_beingPlayed(true);
	auto card = selected_card;
	auto sqe = Sequence::create(DelayTime::create(0.3f),MoveTo::create(0.3f, this->mid_slot_points[1]), CallFunc::create([card] {((Card *)card)->flipToFrontside(); }), nullptr);
	card->runAction(sqe);
	TCGStatus::getInstance()->playerTwoPlayed(true);//so we're going to decision layer.
	return true;
}

bool Moves::round_to(int player_no)
{
	vector<cocos2d::Sprite *> *mid_cards = StageData::getInstance()->get_MiddleStackInstance(); 
	auto &mc = mid_cards;//alias.
	//auto evt_intrv = CallFunc::create([this] {((Gamescene *)this->get_current_affecting_scene())->util_event_intervention_player_one(false); });
	if (player_no == 1) {
		for (int i = 0; i < mc->size(); i++) {
			//(*(mc->begin() + i))->runAction(Sequence::create(DelayTime::create(2.0f), MoveTo::create(0.3f, Vec2(this->vs.width + this->origin.x, 0)),evt_intrv, nullptr));
			cocos2d::log("card %d top player %d", i, player_no);
		}
	}
	else if (player_no == 2) {
		for (int i = 0; i < mc->size(); i++) {
			//(*(mc->begin() + i))->runAction(Sequence::create(DelayTime::create(2.0f), MoveTo::create(0.3f, Vec2(this->vs.width + this->origin.x, this->vs.height + this->origin.y)), evt_intrv, nullptr));
			cocos2d::log("card %d top player %d", i, player_no);
		}
	}

	
	return true;
}

cocos2d::Scene * Moves::get_current_affecting_scene()
{
	return this->gamescene;
}
