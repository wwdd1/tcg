#ifndef __CARDPACK_H__
#define __CARDPACK_H__

#include "cocos2d.h"
#include "Item.h"
#include <iostream>
#include <string>

#define PREDIR "default/assets/cp/"

USING_NS_CC;
using namespace std;

enum class CARDPACK{
	SPECIAL,
	HYPER,
	MAIN,
	SUB
};

class CardPack : public cocos2d::Sprite , public Item{
public :
	CardPack(unsigned int i1, const char *u1, unsigned int i2, ITEMTYPE j) : Item(i1, u1, i2, j) {};

	static cocos2d::Sprite *create(CARDPACK type);
	static cocos2d::Sprite *create(CARDPACK type,std::string unique_id,unsigned int price);
	CARDPACK subtype;
	void attach_design();
	Sprite *get_static_sprite();
private:

};

#endif
