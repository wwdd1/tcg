#ifndef __GAMESTARTADAPTER_H__
#define __GAMESTARTADAPTER_H__

#include <iostream>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <string>
#include "Player.h"
#include "GameStartReviewScene.h"
#include "TMainMenu.h"

enum class GAMETYPE {
	SOLO_GAME,
	RANKED_GAME
};

USING_NS_CC;
class GameStartAdapter : cocos2d::LayerColor{
public:
	GameStartAdapter(){}
	static cocos2d::Scene* createScene();
	static GAMETYPE gametype;
	virtual bool init();
	CREATE_FUNC(GameStartAdapter);

private:
	Size visibleSize;
	Vec2 origin;

	//scene designers
	void soloGame();
	void rankedGame();

	//SOLO GAME METHODS
	void TouchEventCasual(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void TouchEventHard(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	void TouchEventImpossible(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	//RANKED GAME METHODS
};

#endif