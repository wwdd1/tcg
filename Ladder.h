#ifndef __LADDER_H__
#define __LADDER_H__

#include "cocos2d.h"
#include "Card.h"


class Ladderscene : public cocos2d::LayerColor
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(Ladderscene);
private:
	Size visibleSize;
	Vec2 origin;
	void onlineLadderSupply();
	void offlineMode();
};

#endif
