#ifndef __LOCALDB_H__
#define __LOCALDB_H__

#include "cocos2d.h"
#include "CardList.h"
#include "Card.h"
#include "sqlite3.h"
#include "PlayerOne.h"
#include <stdio.h>
#include <cstdlib>

#define CARD_DB_DIR "db/merchandise.db3"
#define USER_DB_DIR "db/occupant.db3"
#define SHOP_DB_DIR "db/deal.db3"

using namespace std;
using namespace cocos2d;

struct MarketItemDbData {
	std::string unique_id;
	std::string name;
	unsigned int price_tp;
	unsigned int price;
};

class LocalDb
{
public:
	LocalDb();
	bool f_check();
	bool f_init();
	int get_totalCount();
	CardList *deckDataFromLocal(std::map<int, int> *);
	bool check_user();
	bool save_user_entry(bool update_required = false);
	bool get_player_sh();
	bool save_player_sh(std::vector<CardList::oneCard> *sh_ins);
	bool get_single_card(CardList::oneCard *);
	bool get_single_card_po(CardList::oneCard *);
	bool get_marketItems_cardpack(std::map<int,MarketItemDbData *> *);
private:
	std::string get_queryofcolor(int ,int,int = -1);
	std::string table_name(int);
};

#endif // __HELLOWORLD_SCENE_H__
