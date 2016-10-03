#ifndef __GAMESTARTREVIEW_H__
#define __GAMESTARTREVIEW_H__

#include <iostream>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <string>
#include <sstream>
#include "PlayerAI.h"
#include "TMainMenu.h"
#include "Gamescene.h"

inline std::string toString(unsigned int i) { std::ostringstream s; s << i; return s.str(); }

enum class SOLOGAMETYPE {
	CASUAL,
	HARD,
	IMPOSSIBLE
};

enum class DIFFICULTY {
	EASY,
	NORMAL,
	HARD,
	HUMAN
};

USING_NS_CC;
class PlayerAI;
class GameStartReview : cocos2d::LayerColor{
public:
	GameStartReview(){}
	static cocos2d::Scene* createScene(SOLOGAMETYPE sologametype, std::map<DIFFICULTY, std::vector<std::string> *> *rivals ,int towerIndex = 0 , Player *opponent = nullptr);
	virtual bool init();
	static GameStartReview *create();

	//setters
	void set_soloGameType(SOLOGAMETYPE type) { sologametype = type; };
	void set_towerMap(std::map<DIFFICULTY, std::vector<std::string> *> *map) { towerMap = map; }
	void set_towerIndex(int index) { towerIndex = index; };

private:
	//void onEnter();

	//draw scene methods
	void drawSoloScene();	//solo games
	void drawRankedScene();	//ranked games

	Size visibleSize;
	Vec2 origin;

	//SOLO GAME OBJECTS
	SOLOGAMETYPE sologametype;
	std::map<DIFFICULTY, std::vector<std::string> *> *towerMap = nullptr;
	int towerIndex = 0;
	int opponentCount = 0;

	PlayerAI *createOpponent();
	void drawOpponentDetail(Player *opponent, DIFFICULTY d, float y_low, float y_high); //this method draws right side of screen.

	//Play Button Callback
	void TouchEventPlay(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

};

#endif