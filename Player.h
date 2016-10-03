#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include <string>
#include <sstream>
#include <cocos2d.h>

using namespace std;
using namespace cocos2d;

class Player {
public:
	Player();
	std::string get_username();
	unsigned int get_uid();
	unsigned int get_tp();
	unsigned int get_xp();
	unsigned int get_level();
	unsigned int get_win_count();
	unsigned int get_lost_count();
	unsigned int get_pts();
	unsigned int get_slots();
	cocos2d::Sprite *get_level_badge();
	bool update_tp();
	std::string toString(int val);
	
	virtual std::string typeof() { return "Player"; }
	
protected:
	unsigned int uid = -1;
	std::string username;
	unsigned int tp = 500;
	unsigned int xp = 0;
	unsigned int level = 1;
	unsigned int pts = 0;
	unsigned int win_count = 0;
	unsigned int lost_count = 0;
	unsigned int slots = 8;
};

#endif // __HELLOWORLD_SCENE_H__
