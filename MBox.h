#ifndef __MBOX_H__
#define __MBOX_H__

#include "cocos2d.h"
#include "ui\CocosGUI.h"
#include <iostream>
#include <string.h>

USING_NS_CC;

class MBox{
public:
	MBox(const char *t,const char *b1 = nullptr,const char *b2 = nullptr,cocos2d::Color4B color = Color4B(240,240,240,255));
	void callback_b1(void (&f)());
	void callback_b2(void (&f)());
	void release(cocos2d::Scene *scene);
	void cleanup();
	cocos2d::LayerColor *get_layer();
private:
	Size visibleSize;
	Vec2 origin;

	bool mbox_prepare();
	cocos2d::LayerColor *window;
	cocos2d::LayerColor *blur;
	cocos2d::Color4B win_color;
	const char *text;
	cocos2d::ui::Button *button_1;
	const char *button_1_text;
	cocos2d::ui::Button *button_2;
	const char *button_2_text;
};

#endif