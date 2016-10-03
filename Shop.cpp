#include "Shop.h"

USING_NS_CC;
using namespace std;
using namespace ui;

MarketPlace *MarketPlace::ref;
cocos2d::Size MarketPlace::ShopItem::main_layer_size;

Scene* MarketPlace::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MarketPlace::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MarketPlace::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !LayerColor::init() )
    {
        return false;
    }
    
	ref = this;
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

	/*Texture2D::TexParams bgpTexp = { GL_REPEAT,GL_REPEAT,GL_REPEAT,GL_REPEAT };
	auto bgImg = Sprite::create("default/bg/bgshop.jpg", Rect(0, 0, visibleSize.width, visibleSize.height));
	bgImg->getTexture()->setTexParameters(&bgpTexp);
	this->addChild(bgImg);
	bgImg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));*/

	LayerColor::initWithColor(Color4B(Color3B(15,15,15)));

	player = PlayerOne::getInstance();

	this->main_layer = LayerColor::create(Color4B(Color3B(20, 20, 20)));
	main_layer->setContentSize(Size(visibleSize.width*0.8f,visibleSize.height/1.7f));
	main_layer->ignoreAnchorPointForPosition(false);
	this->addChild(main_layer);
	main_layer->setPosition(Vec2(visibleSize.width/2 + origin.x , visibleSize.height/2 + origin.y));
	//for shop items sizing.
	ShopItem::main_layer_size = main_layer->getContentSize();


	//return to main menu button
	auto home_btn = Button::create("default/btn/home.png", "default/btn/home.png");
	this->addChild(home_btn,9999);
	home_btn->setPosition(Vec2(visibleSize.width + origin.x - (home_btn->getContentSize().width*0.8f), visibleSize.height + origin.y - (home_btn->getContentSize().height*0.8f)));

	home_btn->addTouchEventListener([&, this](Ref* sender, Widget::TouchEventType type) {
		switch (type) {
		case cocos2d::ui::Widget::TouchEventType::ENDED:
			Director::getInstance()->popScene();
			break;
		}
	});

	//coin o'meter
	auto coin_layer = LayerColor::create(Color4B(Color3B(242, 278, 71)));
	auto coin_layer_icon = Sprite::create("default/icon/coin.png");
	coin_label = Label::createWithTTF(player->toString(player->get_tp()), "fonts/OpenSans-Bold.ttf", 30*0.75);
	coin_label->setColor(Color3B::WHITE);
	coin_layer->setContentSize(Size(coin_layer_icon->getContentSize().width + coin_label->getContentSize().width + 20, coin_layer_icon->getContentSize().height));

	this->addChild(coin_layer, 9999);
	coin_layer->setPosition(Vec2(main_layer->getBoundingBox().getMinX(), main_layer->getBoundingBox().getMaxY() + 10));
	coin_layer->addChild(coin_layer_icon);
	coin_layer->addChild(coin_label);
	coin_layer_icon->setAnchorPoint(Vec2(0, 0.5f));
	coin_layer_icon->setPosition(Vec2(0, coin_layer->getContentSize().height / 2));
	coin_label->setAnchorPoint(Vec2(1.0f, 0.5f));
	coin_label->setPosition(Vec2(coin_layer->getContentSize().width - 10, coin_layer->getContentSize().height / 2));

	//nickname label
	auto nickname_layer = LayerColor::create(Color4B(Color3B(54,103,150)));
	auto nickname_lbl = Label::createWithTTF(player->get_username(), "fonts/OpenSans-Light.ttf", 30*0.75);
	auto slot_icon = Sprite::create("default/icon/slot.png");
	nickname_layer->setContentSize(Size(slot_icon->getContentSize().width + nickname_lbl->getContentSize().width + 20, slot_icon->getContentSize().height));
	nickname_layer->addChild(slot_icon);
	slot_icon->setAnchorPoint(Vec2(0,0.5f));
	slot_icon->setPosition(0, nickname_layer->getContentSize().height/2);
	this->addChild(nickname_layer);
	nickname_layer->setAnchorPoint(Vec2(0,0));
	nickname_layer->setPosition(Vec2(coin_layer->getBoundingBox().getMaxX() + 10,coin_layer->getPositionY()));

	nickname_layer->addChild(nickname_lbl);
	nickname_lbl->setAnchorPoint(Vec2(0,0.5f));
	nickname_lbl->setTextColor(Color4B::WHITE);
	nickname_lbl->setPosition(Vec2(slot_icon->getBoundingBox().getMaxX() + 10,slot_icon->getPositionY()));

	//empty slots
	char empsl[25];
	sprintf(empsl,"Empty Slots Left : %d",player->get_empty_slots_count());
	empslc_label = Label::createWithTTF(empsl,"fonts/OpenSans-Light.ttf",20*0.75);
	this->addChild(empslc_label);
	empslc_label->setAnchorPoint(Vec2(1.0f,0));
	empslc_label->setPosition(Vec2(main_layer->getBoundingBox().getMaxX(),main_layer->getBoundingBox().getMaxY() + 5));

	//tab buttons
	auto tabbtn_pack = Button::create("default/btn/shop_tab_btn_packs.png", "default/btn/shop_tab_btn_packs_c.png");
	this->addChild(tabbtn_pack);
	tabbtn_pack->setAnchorPoint(Vec2(0,1.0f));
	tabbtn_pack->setPosition(Vec2(main_layer->getBoundingBox().getMinX(),main_layer->getBoundingBox().getMinY() - 10));
	
	auto tabbtn_slot = Button::create("default/btn/shop_tab_btn_slot.png", "default/btn/shop_tab_btn_slot_c.png");
	this->addChild(tabbtn_slot);
	tabbtn_slot->setAnchorPoint(Vec2(0, 1.0f));
	tabbtn_slot->setPosition(Vec2(tabbtn_pack->getBoundingBox().getMaxX() + 5, main_layer->getBoundingBox().getMinY() - 10));

	//list first 4 cardpack items on sale at main_layer
	prepare_CardpackItems();

	//selected item init
	this->selected_item = nullptr;

	auto listener = cocos2d::EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	auto origin = Director::getInstance()->getVisibleOrigin();
	listener->onTouchBegan = [&](cocos2d::Touch* touch, cocos2d::Event* evt)
	{
		cocos2d::Vec2 p = touch->getLocation() - Vec2(this->shop_item_size.width / 2, this->shop_item_size.height / 2);
		cocos2d::log("%.2f,%.2f",p.x,p.y);
		

		for (int i = 0; i < this->vector_cardpacks->size(); i++) {
			auto obj = this->vector_cardpacks->at(i);
			cocos2d::Rect rect = obj->getBoundingBox();
			if (rect.containsPoint(p))
			{
				obj->setColor(Color3B(197, 238, 238));
				this->selected_item = obj;
				return true; // to indicate that we have consumed it.
			}
		}

		return false; // we did not consume this event, pass thru.
	};

	listener->onTouchEnded = [&](cocos2d::Touch* touch, cocos2d::Event* evt) {
		//PAUSE TOUCH EVENT LISTENER FOR SHOP ITEMS
		Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(this);
		
		if (this->selected_item == nullptr)	return;
		const char *item_name = this->selected_item->get_name();
		unsigned int item_price = this->selected_item->get_item()->get_price();
		char mb_text[100];
		sprintf(mb_text,"Buy %s for %u TP? ARE YOU SURE?",item_name,item_price);
		MBox *mb = new MBox(mb_text, "BUY","CANCEL");
		mb->callback_b1(MarketPlace::btn_BUY_callback);
		mb->callback_b2(MarketPlace::btn_CANCEL_callback);
		mb->release((Scene *)this); // this is scene.
		this->selected_item->setColor(Color3B(50, 187, 166));
	};

	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void MarketPlace::prepare_CardpackItems()
{
	std::map<int, MarketItemDbData *> cardpack_items;
	LocalDb *db = new LocalDb();

	if (db->get_marketItems_cardpack(&cardpack_items)) {
		for (int i = 0; i < cardpack_items.size(); i++) {
			cocos2d::log("%s %s %lu", cardpack_items[i]->unique_id.c_str(), cardpack_items[i]->name.c_str(), cardpack_items[i]->price_tp);
		}
	}

	vector_cardpacks = new std::vector<ShopItem *>();

	/**
	* @IMPORTANT!!!
	* item creation order below is important! it should be in same sequence with @cardpack_items's elements.
	*/
	
	auto item = ShopItem::create(SHOP_ITEM_TYPES::CATEGORY::CAT_CARDPACK, SHOP_ITEM_TYPES::TYPE::PACK_SPECIAL,cardpack_items[0]);
	main_layer->addChild(item);
	item->setPosition(Vec2(0, 0));
	
	vector_cardpacks->push_back((ShopItem *)item);
	
	//shop item size needed for touch bug fix.
	this->shop_item_size = item->getContentSize();
	
	auto item_2 = ShopItem::create(SHOP_ITEM_TYPES::CATEGORY::CAT_CARDPACK, SHOP_ITEM_TYPES::TYPE::PACK_HYPER,cardpack_items[1]);
	this->main_layer->addChild(item_2);
	item_2->setPosition(Vec2(item->getBoundingBox().getMaxX() + 2, 0));
	vector_cardpacks->push_back((ShopItem *)item_2);
	
	auto item_3 = ShopItem::create(SHOP_ITEM_TYPES::CATEGORY::CAT_CARDPACK, SHOP_ITEM_TYPES::TYPE::PACK_MAIN,cardpack_items[2]);
	main_layer->addChild(item_3);
	item_3->setPosition(Vec2(item_2->getBoundingBox().getMaxX() + 2, 0));
	vector_cardpacks->push_back((ShopItem *)item_3);

	auto item_4 = ShopItem::create(SHOP_ITEM_TYPES::CATEGORY::CAT_CARDPACK, SHOP_ITEM_TYPES::TYPE::PACK_SUB, cardpack_items[3]);
	main_layer->addChild(item_4);
	item_4->setPosition(Vec2(item_3->getBoundingBox().getMaxX() + 2, 0));
	vector_cardpacks->push_back((ShopItem *)item_4);
}

void MarketPlace::pack_openning_scene(CardList::oneCard * card_data, CARDPACK cardpack_type)
{
	Card *card = (Card *)Card::create(card_data, true);
	//db operations

	LayerColor *blur = LayerColor::create(Color4B(0, 0, 0, 230));
	blur->setContentSize(Size(visibleSize.width + origin.x, visibleSize.height + origin.y));
	this->addChild(blur, 9999);
	blur->addChild(card);
	card->setPosition(Vec2(blur->getContentSize().width / 2, blur->getContentSize().height / 2));
	cocos2d::log("uniqueId = %s", card->get_uniqueId());
	auto card_anim = CallFunc::create([card]() { cocos2d::log("%s", card->get_uniqueId()); card->runAction(EaseBounceInOut::create(ScaleTo::create(2.0f, 1.5f))); });
	auto cleanup = CallFunc::create([this,blur, card]() {  
		//show card's price top of it.
		Label *price = Label::createWithTTF(player->toString(card->get_price()).c_str(),"fonts/OpenSans-Bold.ttf",30*0.75);
		Sprite *coin_icon = Sprite::create("default/icon/coin.png");
		price->setAnchorPoint(Vec2(0.5f, 0));
		coin_icon->setAnchorPoint(Vec2(1.0f, 0));
		price->addChild(coin_icon);
		coin_icon->setPosition(Vec2(0,price->getPositionY()));

		//add ok button after card appears.
		ui::Button *ok_button = ui::Button::create("default/btn/po_btn_ok.png");
		ok_button->addTouchEventListener([this,blur](Ref* sender, Widget::TouchEventType type) {
			switch (type) {
			case cocos2d::ui::Widget::TouchEventType::ENDED:
				blur->removeFromParentAndCleanup(true);
				Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(this);
				break;
			}
		});
		ok_button->setScale(0);
		blur->addChild(ok_button);
		ok_button->setPosition(Vec2(card->getPositionX(),card->getPositionY() - card->getContentSize().height - 10));
		ok_button->runAction(ScaleTo::create(0.2f,1.0f));
		
		//show price
		price->setScale(0);
		blur->addChild(price);
		price->setPosition(Vec2(card->getPositionX() + coin_icon->getContentSize().width / 2,card->getPositionY() + card->getContentSize().width + 20));
		price->runAction(ScaleTo::create(0.2f,1.0f));
		//add item to inventory.
		PlayerOne::getInstance()->inventory_add_item(card);
	});

	CardPack* pack = (CardPack *)CardPack::create(cardpack_type);
	pack->setPosition(Vec2(blur->getContentSize().width / 2, blur->getContentSize().height / 2));
	pack->setScale(1.5f);

	auto nodeGrid = NodeGrid::create();
	blur->addChild(nodeGrid);
	nodeGrid->addChild(pack);
	nodeGrid->runAction(Sequence::create(EaseBounceInOut::create(Shaky3D::create(1.0f, Size(1, 1), 3, true)), FadeOutTRTiles::create(3.0f, Size(30, 10)), card_anim, DelayTime::create(2.0f), cleanup, nullptr));

	cocos2d::log("%s", __func__);
}

void MarketPlace::btn_BUY_callback() {
	cocos2d::log("MESSAGE WITH CAPITALS!!!");
	Label *coin = ref->coin_label;
	Label *slots_left = ref->empslc_label;
	PlayerOne *player = ref->player;

	if (player->get_empty_slots_count() == 0) {
		cocos2d::log("no empty slots.");
		ref->selected_item = nullptr;
		Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(ref);
		return;
	}

	auto item = ref->selected_item->get_item();

	if (item->get_type() == ITEMTYPE::ITEM_PACK) {
		CARDPACK subtype = ((CardPack *)(item))->subtype;

		float price = item->get_price();
		if (player->checkout_pack(subtype, price)) {
			cocos2d::log("item %d purchased succesfully.", subtype);
			switch (subtype)
			{
			case CARDPACK::SPECIAL:case CARDPACK::HYPER:case CARDPACK::MAIN:case CARDPACK::SUB: {
				PackOpener *p = new PackOpener();
				if (!p->open(subtype)) {
					cocos2d::log("pack couldnt opened!..");
					break;
				}
				CardList::oneCard *card_data = p->get_card_data();
				ref->pack_openning_scene(card_data, subtype);
			}break;

			default: {
				cocos2d::log("error on cardpack type in func %s()", __func__);
			}break;
			}
		}
		else {
			MBox *m = new MBox("NO ENOUGH TP.", "OK");
			m->release((Scene *)ref);
			cocos2d::log("No enough money..");
		}
	}
	ref->selected_item = nullptr;
	coin->setString(player->toString(player->get_tp()));
	char empsl[25];
	sprintf(empsl, "Empty Slots Left : %d", player->get_empty_slots_count());
	slots_left->setString(empsl);
}

void MarketPlace::btn_CANCEL_callback()
{
	ref->selected_item = nullptr;
	Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(ref);
}


/*void MarketPlace::onEnter()
{
	cocos2d::log("%s",__func__);
	return;
	
}
*/

cocos2d::LayerColor * MarketPlace::ShopItem::create( SHOP_ITEM_TYPES::CATEGORY cat, SHOP_ITEM_TYPES::TYPE type, MarketItemDbData *itemdata)
{
	ShopItem *item = new (std::nothrow) ShopItem();
	if (item && item->init()) {
		item->autorelease();
		switch (cat)
		{
		case SHOP_ITEM_TYPES::CATEGORY::CAT_CARDPACK: {
			item->configurePackItem(type, itemdata);
		}break;
		case SHOP_ITEM_TYPES::CATEGORY::CAT_SLOT: {
			item->configureSlotItem(type, itemdata);
		}break;
		default:
			return nullptr;
		}
		item->attachChildren();
		return item;
	}
	CC_SAFE_DELETE(item);
	return nullptr;
}

const char *MarketPlace::ShopItem::get_name()
{
	return this->name.c_str();
}

Item *MarketPlace::ShopItem::get_item()
{
	try {
		auto item = dynamic_cast<Item *>(this->item_ref);
		return item;
	}
	catch (exception e) {
		cocos2d::log("%s",e.what());
		return nullptr;
	}
}

SHOP_ITEM_TYPES::TYPE MarketPlace::ShopItem::get_item_type()
{
	return this->item_type;
}

void MarketPlace::ShopItem::attachChildren()
{
	//definitions
	Label *item_name = Label::createWithTTF(this->name.c_str(), "fonts/OpenSans-Light.ttf", 20);
	Sprite *item = this->item_ref;
	auto item_bg = Sprite::create("default/bg/shop_item_bg.png");
	unsigned int price = ((CardPack *)item)->get_price();
	ostringstream stm;
	stm << price;
	auto price_lbl = Label::createWithTTF(stm.str(), "fonts/OpenSans-Bold.ttf", 30);
	auto price_icon = Sprite::create("default/icon/coin.png");
	Sprite *specs = this->item_specs;

	//placements and confguration
	this->setColor(Color3B(50, 187, 166));
	this->setContentSize(Size(main_layer_size.width / 4, main_layer_size.height));
	this->setOpacity(200);
	item_name->setColor(Color3B::BLACK);
	item_name->setAlignment(TextHAlignment::CENTER);
	item_name->setMaxLineWidth(this->getContentSize().width - 20);
	this->addChild(item_name, 1);
	item_name->setAnchorPoint(Vec2(0.5f,1.0f));
	item_name->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height - 10));
	this->addChild(item_bg);
	item_bg->setOpacity(100);
	this->addChild(item);
	item->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));
	item_bg->setPosition(item->getPosition());
	this->addChild(price_lbl);
	price_lbl->setAnchorPoint(Vec2(0.5f, 0.5f));
	price_lbl->setPosition(Vec2(this->getContentSize().width / 2 + 10, item->getBoundingBox().getMinY() - price_lbl->getContentSize().height/2 - 10));
	price_lbl->setColor(Color3B(30, 30, 30));
	this->addChild(price_icon);
	price_icon->setPosition(Vec2(price_lbl->getBoundingBox().getMinX() - price_icon->getContentSize().width / 3, price_lbl->getPositionY()));
	this->addChild(specs);
	specs->setAnchorPoint(Vec2(1.0f, 0));
	specs->setPosition(Vec2(this->getBoundingBox().getMaxX() - 5, this->getBoundingBox().getMinY() + 5));
}

void MarketPlace::ShopItem::configurePackItem(SHOP_ITEM_TYPES::TYPE type ,MarketItemDbData *itemdata)
{
	switch (type) {
	case SHOP_ITEM_TYPES::TYPE::PACK_SPECIAL: {
		this->item_ref = CardPack::create(CARDPACK::SPECIAL,itemdata->unique_id,itemdata->price_tp);
		this->name = itemdata->name;
		this->item_specs = Sprite::create("default/icon/special_specs.png");
		this->item_type = type;
	}break;
	case SHOP_ITEM_TYPES::TYPE::PACK_HYPER: {
		this->item_ref = CardPack::create(CARDPACK::HYPER, itemdata->unique_id, itemdata->price_tp);
		this->name = itemdata->name;
		this->item_specs = Sprite::create("default/icon/hyper_specs.png");
		this->item_type = type;
	}break;
	case SHOP_ITEM_TYPES::TYPE::PACK_MAIN: {
		this->item_ref = CardPack::create(CARDPACK::MAIN, itemdata->unique_id, itemdata->price_tp);
		this->name = itemdata->name;
		this->item_specs = Sprite::create("default/icon/main_specs.png");
		this->item_type = type;
	}break;
	case SHOP_ITEM_TYPES::TYPE::PACK_SUB: {
		this->item_ref = CardPack::create(CARDPACK::SUB, itemdata->unique_id, itemdata->price_tp);
		this->name = itemdata->name;
		this->item_specs = Sprite::create("default/icon/sub_specs.png");
		this->item_type = type;
	}break;
	default: {
		Director::getInstance()->popScene();
	}break;
	}
}

void MarketPlace::ShopItem::configureSlotItem(SHOP_ITEM_TYPES::TYPE type , MarketItemDbData *itemdata)
{
}