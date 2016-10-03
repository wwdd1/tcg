#include "PlayerAI.h"

PlayerAI::PlayerAI(std::string name) : Player()
{
	this->username = name;
	this->level = 0;
	this->lost_count = 0;
	this->pts = 0;
	this->win_count = 0;
	this->xp = 0;
	this->tp = 0;
	this->uid = 0;
}

