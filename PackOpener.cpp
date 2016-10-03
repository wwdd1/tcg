#include "PackOpener.h"

/**
* PACK RATINGS
*  Blue   : 8 - 10
*  Yellow : 4 - 9
*  Green  : 2 - 6
*  Silver : 1 - 4
*  Bronze : 1 - 3
*/

bool PackOpener::open(CARDPACK type)
{
	switch (type)
	{
	case CARDPACK::SPECIAL:
		this->cpspecial();
		return true;
	case CARDPACK::HYPER:
		this->cphyper();
		return true;
	case CARDPACK::MAIN:
		this->cpmain();
		return true;
	case CARDPACK::SUB:
		this->cpsub();
		return true;
	default:
		cocos2d::log("cannot resolve card pack type in function %s()",__func__);
		return false;
	}
}

CardList::oneCard * PackOpener::get_card_data()
{
	if (card_data.card_id != -1)
		return &this->card_data;
	cocos2d::log("first, you have to call open() method to fill oneCard data.");
	return nullptr;
}

void PackOpener::init()
{
	//here is our distribution object. value should be nearby generated number which we want mostly.
	//it means 3.5 for now.
	unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
	this->e.seed(seed);
	this->card_data.card_id = -1;
	//for making guarantee to if seed is changed or not for multiple operations we should store it and check it everytime.
}

bool PackOpener::cpspecial()
{
	//min : orange 80
	//max : orange 99
	init();

	return false;
}

bool PackOpener::cphyper()
{
	//min : yellow 83 (all yellows)
	//max : blue 99	  (all blues)
	init();
	//poisson d for color variable
	std::poisson_distribution<int> p(3.0f);

	int max_idx_1 = 0;

	int pi[5] = {};

	for (int i = 0; i<TRIES; ++i) {
		int number = p(e) % 5;
		++pi[number];
		if (pi[number] > pi[max_idx_1])
			max_idx_1 = number;
	}

	int min_rate = -1,max_rate = -1;

	switch (max_idx_1) {
	case 0:case 1: {
		this->card_data.color = CARDCOLOR_BLUE;
	}break;
	case 2:case 3:case 4: {
		this->card_data.color = CARDCOLOR_YELLOW;
	}break;
	}

	int seed = (int)((pi[max_idx_1] * TRIES) / PERCENT);
	e.seed(seed);
	std::uniform_int_distribution<int> d(MIN_RATE(card_data.color),MAX_RATE(card_data.color));
	this->card_data.rate = d(e);

	if (!db->get_single_card_po(&this->card_data)) {
		cocos2d::log("couldnt fill card_data from localdb. func : %s", __func__);
		return false;
	}
		
	return true;
}

bool PackOpener::cpmain()
{
	//min : green 80 
	//max : yellow 89 (all yellows)
	init();
	//poisson d for color variable
	std::poisson_distribution<int> p(3.0f);

	int max_idx_1 = 0;

	int pi[5] = {};

	for (int i = 0; i<TRIES; ++i) {
		int number = p(e) % 5;
		++pi[number];
		if (pi[number] > pi[max_idx_1])
			max_idx_1 = number;
	}

	int min_rate = -1, max_rate = -1;

	switch (max_idx_1) {
	case 0:case 1: {
		this->card_data.color = CARDCOLOR_YELLOW;
	}break;
	case 2:case 3:case 4: {
		this->card_data.color = CARDCOLOR_GREEN;
	}break;
	}

	int seed = (int)((pi[max_idx_1] * TRIES) / PERCENT);
	e.seed(seed);
	std::uniform_int_distribution<int> d(MIN_RATE(card_data.color), MAX_RATE(card_data.color));
	this->card_data.rate = d(e);

	if (!db->get_single_card_po(&this->card_data)) {
		cocos2d::log("couldnt fill card_data from localdb. func : %s", __func__);
		return false;
	}

	return true;
}

bool PackOpener::cpsub()
{
	//min : bronze 70
	//max : silver 79
	init();
	//poisson d for color variable
	std::uniform_int_distribution<int> c(0, 1);

	int color_idx = c(e);

	int min_rate = -1, max_rate = -1;

	switch (color_idx) {
	case 0: {
		this->card_data.color = CARDCOLOR_SILVER;
	}break;
	case 1:{
		this->card_data.color = CARDCOLOR_BRONZE;
	}break;
	}

	int seed = (int)((color_idx * TRIES) / PERCENT);
	e.seed(seed);
	std::uniform_int_distribution<int> d(MIN_RATE(card_data.color), MAX_RATE(card_data.color));
	this->card_data.rate = d(e);

	if (!db->get_single_card_po(&this->card_data)) {
		cocos2d::log("couldnt fill card_data from localdb. func : %s", __func__);
		return false;
	}

	return true;

}
