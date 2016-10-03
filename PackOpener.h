#ifndef __PACKOPENER_H__
#define __PACKOPENER_H__

#include "cocos2d.h"
#include "Card.h"
#include "CardList.h"
#include "CardPack.h"
#include "LocalDb.h"
#include <iostream>
#include <random>
#include <chrono>
#include <ctime>

#define TRIES 230	//gotten by trial and error
#define PERCENT 100 //percent

USING_NS_CC;
using namespace std;

inline int MAX_RATE(int x) { switch (x) { case 0:return 3; case 1:return 4; case 2:return 6; case 3:return 9; case 4:return 10; default:return -1; } }
inline int MIN_RATE(int x) { switch (x) { case 0:return 1; case 1:return 1; case 2:return 2; case 3:return 4; case 4:return 8;  default:return -1; } }

class PackOpener {
public:
	PackOpener() { this->card_data.card_id = -1; db = new LocalDb(); }
	bool open(CARDPACK type);
	CardList::oneCard *get_card_data();

private:
	void init();

	bool cpspecial();
	bool cphyper();
	bool cpmain();
	bool cpsub();

	CardList::oneCard card_data;
	std::mt19937 e;
	//300 is the best experiment count for a certain result
	LocalDb *db;
};

#endif