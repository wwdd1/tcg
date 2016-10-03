#ifndef __TMAINMENU_H__
#define __TMAINMENU_H__

#include "cocos2d.h"
#include "Card.h"
#include "ClientManager.h"
#include "PlayerOne.h"
#include "Inventoryscene.h"
#include "Ladder.h"
#include <ui/CocosGUI.h>
#include <iostream>
#include <string>
#include "Shop.h"
#include "GameStartAdapter.h"

#define MENU_ELM_COUNT 2

class TMainMenu : public cocos2d::LayerColor
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(TMainMenu);
private:
	cocos2d::Menu* _menu;
	Card *card;
	bool createMenu();
	void startTheGame();
	void exitApp();
};

#endif // __HELLOWORLD_SCENE_H__
