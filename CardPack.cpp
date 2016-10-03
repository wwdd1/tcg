#include "CardPack.h"

cocos2d::Sprite *CardPack::create(CARDPACK pack_type)
{
	CardPack *sprite = new (std::nothrow) CardPack(3000, "UID01", 0, ITEMTYPE::ITEM_PACK);
	if (sprite && sprite->init()) {
		sprite->autorelease();
		sprite->subtype = pack_type;
		sprite->attach_design();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

cocos2d::Sprite * CardPack::create(CARDPACK pack_type, std::string unique_id, unsigned int price)
{
	CardPack *sprite = new (std::nothrow) CardPack(3000, unique_id.c_str(), price, ITEMTYPE::ITEM_PACK);
	if (sprite && sprite->init()) {
		sprite->autorelease();
		sprite->subtype = pack_type;
		sprite->attach_design();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

void CardPack::attach_design()
{
	char pre_dir[50];
	switch (this->subtype) {
	case CARDPACK::SPECIAL:
		sprintf(pre_dir,"%sspecial",PREDIR);
		break;
	case CARDPACK::HYPER:
		sprintf(pre_dir, "%shyper",PREDIR);
		break;
	case CARDPACK::MAIN:
		sprintf(pre_dir, "%smain",PREDIR);
		break;
	case CARDPACK::SUB:
		sprintf(pre_dir, "%ssub",PREDIR);
		break;
	default:
		cocos2d::log("UNDEFINED CARDPACK TYPE!!");
		return;
	}
	auto size_delegate = Sprite::create("default/assets/cp/hyper/b0.png");
	Size contentsize = size_delegate->getContentSize();

	//30 means we have 30 frames in spritesheet
	Vector<SpriteFrame *> pack(30);
	char str[100] = { 0 };
	for (int i = 0; i < 30; i++) {
		sprintf(str, "%s/b%d.png",pre_dir, i);
		auto frame = SpriteFrame::create(str, Rect(0, 0, contentsize.width, contentsize.height));
		pack.pushBack(frame);
	}

	auto card_pack_blue = Animation::createWithSpriteFrames(pack, 0.03f);
	auto anim = Animate::create(card_pack_blue);
	auto sq = Sequence::create(anim, DelayTime::create(2.0f), nullptr);
	this->setContentSize(contentsize);
	this->setAnchorPoint(Vec2(0.5f,0.5f));
	this->runAction(RepeatForever::create(sq));
}

Sprite * CardPack::get_static_sprite()
{
	switch (this->subtype)
	{
	case CARDPACK::HYPER: {
		Sprite *sprite = Sprite::create("default/assets/cp/hyper/b29.png");
		return (sprite) ? sprite : nullptr;
	}
	default:
		break;
	}
	return nullptr;
}
