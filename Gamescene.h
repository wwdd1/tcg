#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__

#include <iostream>
#include <string>
#include "cocos2d.h"
#include "LocalDb.h"
#include "Card.h"
#include "CardList.h"
#include "PlayerOne.h"
#include "PlayerAI.h"

/*
#include "Moves.h"
#include "TCGStatus.h"
#include "StageData.h"
*/
USING_NS_CC;
class Gamescene : public cocos2d::LayerColor
{
public:
	Gamescene(Player *player);
    static cocos2d::Scene* createScene(Player *player);
	static cocos2d::LayerColor *create(Player *player);
    virtual bool init();
    

    // implement the "static create()" method manually
    CREATE_FUNC(Gamescene);
private:
	//objects
	PlayerOne* _me;
	Player * _opponent;


	//PlayerOne Plays.
	//PlayerTwo Plays.
	//Deciser calls.
	//Results occurs.
};

#endif
