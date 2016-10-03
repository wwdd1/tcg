#include "Player.h"

Player::Player() {
}

std::string Player::get_username()
{
	return this->username;
}

unsigned int Player::get_uid()
{
	return (this->uid) ? this->uid : -1;
}

unsigned int Player::get_tp()
{
	return this->tp;
}

unsigned int Player::get_xp()
{
	return this->xp;
}

unsigned int Player::get_level()
{
	return this->level;
}

unsigned int Player::get_win_count()
{
	return this->win_count;
}

unsigned int Player::get_lost_count()
{
	return this->lost_count;
}

unsigned int Player::get_pts()
{
	return this->pts;
}

unsigned int Player::get_slots()
{
	return this->slots;
}

Sprite * Player::get_level_badge()
{
	std::string lvl = "default/icon/level_icons/" + this->toString(this->get_level()) + ".png";
	Sprite *badge = Sprite::create(lvl.c_str());
	return badge;
}

bool Player::update_tp()
{
	//update tp locally.
	return false;
}

std::string Player::toString(int val)
{
	ostringstream s;
	s << val;
	return s.str();
}

