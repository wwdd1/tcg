#ifndef __SHOP_H__
#define __SHOP_H__

#include "cocos2d.h"
#include <iostream>
#include <string>
#include "PlayerOne.h"
#include "ClientManager.h"
#include "Inventoryscene.h"
#include "Card.h"
#include "CardPack.h"
#include <ui/CocosGUI.h>
#include "MBox.h"
#include "Item.h"
#include "LocalDb.h"
#include "PackOpener.h"

struct SHOP_ITEM_TYPES {
	enum class CATEGORY {
		CAT_CARDPACK,
		CAT_SLOT
	};
	enum class TYPE {
		PACK_SPECIAL,
		PACK_HYPER,
		PACK_MAIN,
		PACK_SUB
	};
};

class MarketPlace : public cocos2d::LayerColor
{
public:

    static cocos2d::Scene* createScene();
    virtual bool init();
	//void onEnter();

	Size visibleSize;
	Vec2 origin;

    // implement the "static create()" method manually
    CREATE_FUNC(MarketPlace);
private:
	 //empty slot count title for label.
	//prepare funcs
	void prepare_CardpackItems();
	LayerColor *main_layer;

	class ShopItem : public cocos2d::LayerColor {
	public:
		ShopItem() {  }
		static Size main_layer_size;
		static cocos2d::LayerColor *create(SHOP_ITEM_TYPES::CATEGORY cat,SHOP_ITEM_TYPES::TYPE type, MarketItemDbData *itemdata);
		const char *get_name();
		Item *get_item();
		SHOP_ITEM_TYPES::TYPE get_item_type();
	private:
		void attachChildren();
		void configurePackItem(SHOP_ITEM_TYPES::TYPE type, MarketItemDbData *itemdata);
		void configureSlotItem(SHOP_ITEM_TYPES::TYPE type, MarketItemDbData *itemdata);

		std::string name;
		cocos2d::Sprite * item_ref;
		cocos2d::Sprite * item_specs;
		SHOP_ITEM_TYPES::TYPE item_type;
	};
	//SHOP ITEM

	Label *empslc_label; // empty slots counter label
	Label *coin_label;   // coin'o meter label
	Size shop_item_size;
	ShopItem *selected_item;
	SHOP_ITEM_TYPES selected_item_type;
	PlayerOne *player;
	std::vector<ShopItem *> *vector_cardpacks;
	void pack_openning_scene(CardList::oneCard *card_data,CARDPACK cardpack_type);


	static MarketPlace* ref;
	static void btn_BUY_callback();
	static void btn_CANCEL_callback();
};

#endif
