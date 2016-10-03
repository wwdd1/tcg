#include "StageData.h"

USING_NS_CC;
StageData * StageData::instance;
map<int,int> *StageData::get_MiddleStackCardAvgs()
{
	if(this->areAnyCardsOnMiddle())
		return nullptr;
	map<int, int> *_data = new map<int,int>();
	map<int, int> &data = *_data;
	int count = this->playdata_cardstacks->cards_played_middle_stack.size();

	for (int i = 0; i < count; i++)
		data[i] = ((Card *)(*(this->playdata_cardstacks->cards_played_middle_stack.begin() + i)))->get_data()->avg;
	
	return _data;
}
bool StageData::areAnyCardsOnMiddle()
{
	return (this->playdata_cardstacks->cards_played_middle_stack.empty());
}

void StageData::addToMiddleStack(cocos2d::Sprite *played_card)
{
	this->playdata_cardstacks->cards_played_middle_stack.push_back(played_card);
}

void StageData::emptyTheMiddleStack()
{
	this->playdata_cardstacks->cards_played_middle_stack.clear();
}

vector<cocos2d::Sprite*>* StageData::get_MiddleStackInstance()
{
	vector<cocos2d::Sprite*> *msi = new vector<cocos2d::Sprite*>();
	msi = &(this->playdata_cardstacks->cards_played_middle_stack);
	return msi;
}

void StageData::midToPlayerOne()
{
	vector<cocos2d::Sprite *>::iterator card = playdata_cardstacks->cards_played_middle_stack.begin();
	for (; card != playdata_cardstacks->cards_played_middle_stack.end(); card++) {
		playdata_cardstacks->cards_player_one_earned.push_back(*card);
	}
	this->emptyTheMiddleStack();
}

void StageData::midToPlayerTwo()
{
	vector<cocos2d::Sprite *>::iterator card = playdata_cardstacks->cards_played_middle_stack.begin();
	for (; card != playdata_cardstacks->cards_played_middle_stack.end(); card++) {
		playdata_cardstacks->cards_player_two_earned.push_back(*card);
	}
	this->emptyTheMiddleStack();
}

void StageData::addEmptySlotonHand(int player, int slot)
{
	if (player == 1) {
		this->playdata_cardstacks->empty_slots_player_one.push_back(slot);
	}
	else if (player == 2) {
		this->playdata_cardstacks->empty_slots_player_two.push_back(slot);
	}
}

vector<int> *StageData::get_EmptySlotsOnHand(int player)
{
	vector<int> *slots = new vector<int>();
	if (player == 1) {
		slots = &this->playdata_cardstacks->empty_slots_player_one;
	}
	else if (player == 2) {
		slots = &this->playdata_cardstacks->empty_slots_player_two;
	}
	return slots;
}

void StageData::emptyAllEmptySlots()
{
	this->playdata_cardstacks->empty_slots_player_one.clear();
	this->playdata_cardstacks->empty_slots_player_two.clear();
}

StageData::StageData() {
	this->playdata_cardstacks = new PlayData_CardStacks;
}

StageData * StageData::getInstance() {
	if (!instance)
		instance = new StageData();
	return instance;
}
