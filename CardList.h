#ifndef __CARDLIST_H__
#define __CARDLIST_H__

#include "cocos2d.h"
#include <cstdlib>
#include <iostream>

using namespace cocos2d;
using namespace std;

class CardList {
public:
	CardList(){}
	struct ocr{
		unsigned int card_id;
		int color;
		int avg;
		int rate;
		std::string name;
		std::string position;
		std::string land;
		std::string team;
		unsigned int price;
		struct ocr *prevNode;
	};
	typedef struct ocr oneCard;

	bool add(oneCard *);
	oneCard *top();
	oneCard *pop();
	bool isEmpty();
	int size();
	void disorder();
private:
	oneCard * firstNode = NULL;
	oneCard * topNode = NULL;
	int sizeoflist = 0;
	void copy(oneCard *,oneCard *);
};

#endif
