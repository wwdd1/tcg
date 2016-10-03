#ifndef __PLAYERHUMAN_H__
#define __PLAYERHUMAN_H__

#include <iostream>
#include <string>
#include "Player.h"

class PlayerHuman : public Player{
public:
	PlayerHuman();
	virtual std::string typeof() { return "PlayerHuman"; };
private:
};

#endif