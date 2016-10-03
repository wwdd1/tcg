#ifndef __PLAYERONE_H__
#define __PLAYERONE_H__

#include <iostream>
#include <string>
#include <vector>
#include "Card.h"
#include "Player.h"
#include "cocos2d.h"
#include "CardList.h"
#include "CardPack.h"

class Card;
class PlayerOne : public Player
{
private:
	struct init_flags {
		bool player;
		bool inventory;
	};
	PlayerOne();
	static PlayerOne *instance;
	std::vector<CardList::oneCard> sh;
	std::vector<CardList::oneCard> inventory;
	struct init_flags iflags;
public:
	std::vector<CardList::oneCard> *get_sh_ref();
	static PlayerOne *getInstance();
	bool init_local();
	void init_data(
		unsigned int uid,
		const char *uname,
		unsigned int tp,
		unsigned int xp,
		unsigned int level,
		unsigned int win_count,
		unsigned int lost_count,
		unsigned int pts,
		int slots
		);
	virtual std::string typeof() { return "PlayerOne"; }

	void inventory_init(std::vector<CardList::oneCard> *inv);
	std::vector<CardList::oneCard> *inventory_get_vector_copy();
	int inventory_add_item(Card *item);
	bool inventory_delete_item();
	int get_empty_slots_count();


	//checkout funcs
	bool checkout_pack(CARDPACK type, int price);
	
	//sell funcs
	bool sell_card(Card *card);
};

#endif 
