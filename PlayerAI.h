#ifndef __PLAYERAI_H__
#define __PLAYERAI_H__

#include <iostream>
#include <string>
#include "Player.h"
#include "GameStartAdapter.h"

class PlayerAI : public Player{
public:
	PlayerAI(std::string name);
	virtual std::string typeof() { return "PlayerAI"; };
private:
};

#endif