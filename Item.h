#ifndef __ITEM_H__
#define __ITEM_H__

#include <iostream>
#include <string>

using namespace std;

enum class ITEMTYPE{
	ITEM_CARD,
	ITEM_PACK
};

class Item{
public:
	Item(unsigned int id , const char *unique_id , unsigned int price , ITEMTYPE type )
		: _Id(id) , _Unique_Id(unique_id) ,_Price(price) , _Type(type)
	{
	};

	const unsigned int get_id();
	const unsigned int get_price();
	const char * get_uniqueId();
	const ITEMTYPE get_type();

private:
	const unsigned int _Id;
	const char *_Unique_Id;
	const unsigned int _Price;
	const ITEMTYPE _Type;
};

#endif
