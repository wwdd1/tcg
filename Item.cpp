#include "Item.h"

const unsigned int Item::get_id()
{
	return this->_Id;
}

const unsigned int Item::get_price()
{
	return _Price;
}

const char * Item::get_uniqueId()
{
	return this->_Unique_Id;
}

const ITEMTYPE Item::get_type()
{
	return this->_Type;
}
