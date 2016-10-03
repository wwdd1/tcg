#ifndef __STAGEDATA_H__
#define __STAGEDATA_H__

#include "cocos2d.h"
#include "CardList.h"
#include "Card.h"
#include <iostream>

using namespace std;

class StageData {
public:
	static StageData *getInstance();
	typedef struct d1 {
		vector<cocos2d::Sprite *> cards_played_middle_stack; //cards on middle during a round
		vector<cocos2d::Sprite *> cards_player_one_earned; //cards earned by players during a single game
		vector<cocos2d::Sprite *> cards_player_two_earned;
		vector<int> empty_slots_player_one;
		vector<int> empty_slots_player_two;
	}PlayData_CardStacks;

	map<int,int> *get_MiddleStackCardAvgs();
	bool areAnyCardsOnMiddle();
	void addToMiddleStack(cocos2d::Sprite *);
	void emptyTheMiddleStack();
	vector<cocos2d::Sprite *>* get_MiddleStackInstance();
	void midToPlayerOne();
	void midToPlayerTwo();

	void addEmptySlotonHand(int player,int slot);
	vector<int> *get_EmptySlotsOnHand(int player);
	void emptyAllEmptySlots();
private:
	StageData();
	static StageData *instance;
	PlayData_CardStacks *playdata_cardstacks;
};

#endif
