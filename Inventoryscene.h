#ifndef __INVENTORYSCENE_H__
#define __INVENTORYSCENE_H__

#include "cocos2d.h"
#include "Card.h"
#include <iostream>
#include <typeinfo>
#include <cmath>
#include "PlayerOne.h"
#include "Card.h"
#include "CardPack.h"
#include "CardList.h"
#include "Item.h"
#include <ui\CocosGUI.h>
#include "MBox.h"

class Inventoryscene : public cocos2d::LayerColor
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	enum MenuType { 
		CARD_MENU_INVENTORY,
		CARD_MENU_SH,
		CARD_MENU_PACK
	};
	typedef enum MenuType MenuType;
	// implement the "static create()" method manually
	CREATE_FUNC(Inventoryscene);
private:
	PlayerOne *player;
	bool onTouchBegan(Touch* touch, Event* event);
	void onTouchMoved(Touch* touch, Event* event);
	void onTouchEnded(Touch *touch, Event* event);
	float max_z_order = 0.0f;
	Size card_size;
	Size visibleSize;
	Vec2 origin;
	std::vector<Sprite *> inventory;
	std::vector<Sprite *> starting_hand;
	Sprite *selected_item = nullptr;
	Sprite *empty_slot_instance;
	ui::Button *save_sh_button;
	cocos2d::LayerColor * scroll_layer;
	int scroll_factor_i = 0;
	cocos2d::ui::Button *chv_right;
	cocos2d::ui::Button *chv_left;
	cocos2d::ui::Button *sell;
	int card_count_on_screen = 0;
	float card_margin_h = 0.0f;
	float shift_x = 0.0f;
	Vec2 first_card_position_on_scroller;
	Vec2 last_card_position_on_scroller;

	//changings
	Label *coin_label;

	void load_inventory();
	void prepare_scroller();
	void set_cards_listeners();
	void prepare_starting_hand_visuals();
	void prepare_op_buttons();

	bool scroll_right();
	bool scroll_left();

	void cardMenu_prepare();
	void cardMenu_pop(MenuType type);
	void cardMenu_hide();
	std::map<int, cocos2d::ui::Button *> cardmenu;

	void cardmenu_add();
	void cardmenu_remove();
	void cardmenu_openthepack();
	void cardmenu_sell();
};
#endif
