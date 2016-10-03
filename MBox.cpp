#include "MBox.h"

MBox::MBox(const char *t, const char *b1, const char *b2, cocos2d::Color4B color) :
text(t), button_1_text(b1), button_2_text(b2), win_color(color)
{
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	mbox_prepare();
}

bool MBox::mbox_prepare()
{
	blur = LayerColor::create(Color4B(0, 0, 0, 180));
	blur->setContentSize(Size(visibleSize.width + origin.x,visibleSize.height + origin.y));

	window = LayerColor::create(this->win_color);
	window->ignoreAnchorPointForPosition(false);
	window->setAnchorPoint(Vec2(0.5f, 0.5f));
	window->setContentSize(Size(visibleSize.width / 2, visibleSize.height / 4));
	window->setPosition(blur->getContentSize()/2);
	blur->addChild(window);

	button_1 = ui::Button::create("default/btn/mbox_btn1.png", "default/btn/mbox_btn1_c.png");
	button_1->setTitleFontName("fonts/OpenSans-ExtraBold.ttf");
	button_1->setTitleText(this->button_1_text);
	button_1->setTitleFontSize(20);
	button_1->setAnchorPoint(Vec2(1.0f,0.5f));
	window->addChild(button_1);
	button_1->setPosition(Vec2(window->getContentSize().width - 10, window->getContentSize().height/2));
	

	if (this->button_2_text != nullptr) {
		button_2 = ui::Button::create("default/btn/mbox_btn2.png","default/btn/mbox_btn2_c.png");
		button_2->setTitleFontName("fonts/OpenSans-ExtraBold.ttf");
		button_2->setTitleText(this->button_2_text);
		button_2->setTitleFontSize(20);
		button_2->setAnchorPoint(Vec2(1.0f,1.0f));
		button_1->setAnchorPoint(Vec2(1.0f,0));
		window->addChild(button_2);
		button_1->setPosition(Vec2(window->getContentSize().width - 10 , window->getContentSize().height / 2 + 5));
		button_2->setPosition(Vec2(window->getContentSize().width - 10 , window->getContentSize().height / 2 - 5));
	}

	Label *msg = Label::createWithTTF(this->text,"fonts/OpenSans-Light.ttf",22);
	msg->setAlignment(TextHAlignment::CENTER);
	msg->setTextColor(Color4B::BLACK);
	msg->setMaxLineWidth(window->getContentSize().width - button_1->getContentSize().width * 1.5f);
	window->addChild(msg);
	msg->setAnchorPoint(Vec2(0,0.5f));
	msg->setPosition(Vec2(25,window->getContentSize().height/2));

	return false;
}

void MBox::callback_b1(void(&f)())
{
	button_1->addTouchEventListener([&](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
		switch (type){
		case cocos2d::ui::Widget::TouchEventType::ENDED:
			cocos2d::log("button_1 cb");
			this->blur->removeFromParentAndCleanup(true);
			f();
		break;
		}
	});
}

void MBox::callback_b2(void(&f)())
{
	if (button_2 == nullptr) return;
	button_2->addTouchEventListener([&](cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
		switch (type) {
		case cocos2d::ui::Widget::TouchEventType::ENDED:
			cocos2d::log("button_2 cb");
			this->blur->removeFromParentAndCleanup(true);
			f();
			break;
		}
	});
}

void MBox::release(cocos2d::Scene * scene)
{
	//window->setScale(0);
	scene->addChild(blur);
	//window->runAction(ScaleTo::create(0.1f, 1.0f));
}

void MBox::cleanup()
{
	this->blur->removeFromParentAndCleanup(true);
}

cocos2d::LayerColor * MBox::get_layer()
{
	return blur;
}
