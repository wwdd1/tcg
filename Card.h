#ifndef __CARD_H__
#define __CARD_H__

#include "cocos2d.h"
#include "Item.h"
#include "CardList.h"
#include "TCGStatus.h"
#include "Moves.h"
#include "Gamescene.h"
#include <string>
#include <sstream>

#define FRONTSIDE -1
#define BACKSIDE 1

#define CARDCOLOR_BRONZE 0
#define CARDCOLOR_SILVER 1
#define CARDCOLOR_GREEN 2
#define CARDCOLOR_YELLOW 3
#define CARDCOLOR_BLUE 4
#define CARDCOLOR_ORANGE 5

#define CARDINDEX_BRONZE 0
#define CARDINDEX_SILVER 1
#define CARDINDEX_GREEN 2
#define CARDINDEX_YELLOW 3
#define CARDINDEX_BLUE 4
#define CARDINDEX_ORANGE 5

#define CARDS_DIR "default/assets/c/"
#define TEAMS_DIR "default/assets/j/"
#define LANDS_DIR "default/assets/n/"
#define POSITIONS_DIR "default/assets/p/"

#define CARD_BG "000.png"
#define CARD_BRONZE "0.png"
#define CARD_SILVER "1.png"
#define CARD_GREEN "2.png"
#define CARD_YELLOW "3.png"
#define CARD_BLUE "4.png"
#define CARD_ORANGE "5.png"
#define CARD_EMPTY "empty-slot.png"

class Card : public cocos2d::Sprite , public Item
{
public:
	Card(unsigned int i1, const char *u1, unsigned int i2, ITEMTYPE j) : Item(i1, u1, i2, j) {};
	struct card_data {
		int card_id;
		int color;
		int avg;
		int rate;
		std::string name;
		std::string position;
		std::string land;
		std::string team;
	};
	typedef struct card_data CardData;
	static Sprite *create(CardList::oneCard *,bool);
	static Sprite *create();
	static cocos2d::Size getDefinedContentSize();

	//onTouch event funcs
	void addEvents();
	void suspendEvents();
	void resumeEvents();
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event *event);
	//void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event *event);
	//void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event *event);

	bool flipToBackside();
	bool flipToBackside(bool);
	bool flipToFrontside();
	bool isFrontside();
	void setSelected(bool s);
	bool isSelected();
	
	bool isbeingPlayed();
	void set_beingPlayed(bool);

	bool ishandOn();
	bool isClicked();
	void set_handOn(bool);
	void set_clicked(bool c);
	CardData *get_data();
	void clone_cardinfo(CardList::oneCard *);
	
	int index_on_hand = -1;

	bool operator==(Card &card1) {
		if (this->c->card_id == card1.get_data()->card_id && this->c->color == card1.get_data()->color) {
			return true;
		}
		return false;
	}

protected:
	struct card_data *c;
	bool selected = false;
	bool isFS = true;
	bool bengPl = false;
	bool is_handOn = false;
	bool clicked = false;

	void attachElements(CardList::oneCard *);
	void createEmptyCard();
	void set_card_data(CardList::oneCard * data);
	void z(signed int z);
};

#endif // __HELLOWORLD_SCENE_H__
