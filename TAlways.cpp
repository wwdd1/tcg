#include "TAlways.h"

USING_NS_CC;
using namespace std;
using namespace tinyxml2;
using namespace ui;

Scene* TAlways::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = TAlways::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool TAlways::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

	LayerColor::initWithColor(Color4B(Color3B::BLACK));
	/*
#ifndef XMLCheckResult
#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { CCLOG("XML Error: %i\n", a_eResult); return a_eResult; }
#endif

	/*tinyxml2::XMLDocument xmldoc;
	XMLError eResult = xmldoc.LoadFile("character.xml");
	XMLCheckResult(eResult);

	XMLNode *root = xmldoc.FirstChild();

	/*if (root == nullptr)	CCLOG("xml root is nullptr.");
	else {
		XMLElement *elm = root->FirstChildElement();
		//const char *text = elm->GetText();

		//CCLOG(text);
	}*/

	const char *a_msg = "All visuals and the other materials that hosted in\n the Tournami Card Game are fictional creations.";
	auto audience_message = Label::createWithTTF(a_msg,"fonts/OpenSans-Light.ttf",35);
	audience_message->setColor(Color3B::WHITE);
	audience_message->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2));
	this->addChild(audience_message,0,"audience_message");

	auto loading_animation = Sprite::create("default/icon/spinner.png");
	loading_animation->setAnchorPoint(Vec2(0.5f,0.5f));
	loading_animation->setPosition(Vec2(origin.x + visibleSize.width - this->getBoundingBox().size.width*0.1f,origin.y + this->getBoundingBox().size.height*0.1f));
	this->addChild(loading_animation,1,"loading_animation");

	this->info = Label::createWithTTF("Connecting", "fonts/OpenSans-Regular.ttf",20);
	this->info->setAnchorPoint(Vec2(1.0f,0.5f));
	this->info->setPosition(loading_animation->getPosition() - Vec2(info->getContentSize().width/2,0));
	this->addChild(this->info);

	this->db = new LocalDb();
	this->client = ClientManager::getInstance();
	this->player = PlayerOne::getInstance();

	//check database created before.
	check_localdb_files();
	//localdb tamamdır. servera baglanalım.
	this->server_connection();
	auto sq = Sequence::create(RotateBy::create(2.0f, 360),  nullptr);
	loading_animation->runAction(RepeatForever::create(sq));
	/*
	if (!client->isConnected()//!client->connect_to_server()) {
		if (db->check_user()) {
			//offline modda başlat.
			//this->scheduleOnce(schedule_selector(TAlways::go_to_mainmenu), 2); //2 is scene replace wait time.
		}
	} else {
		cocos2d::log("CONNECTED!!! ");
		if (!db->check_user()) {
			//no user on local db or user logged out from settings menu.
			this->releaseSignupLayer();
			return true;
		}
		if (!client->AuthMyUser(false, player->get_username().c_str(), "PASSWORDHACK!")) {
			//cant authenticate or user not found.
			this->releaseSignupLayer();
			return true;
		}
		else {
			//logged in and gotten all user data from server. Let it replace to main menu.
			this->scheduleOnce(schedule_selector(TAlways::go_to_mainmenu), 2); //2 is scene replace wait time.
			return true;
		}
	}*/
	return true;
}

bool TAlways::ls_button_click(const char * uname, const char * pw)
{
	regex r("[[:alnum:]]+");
	regex p("[[:space:]]+");
	if (!regex_match(uname,r) || regex_match(uname,p) || uname == NULL || strncmp(uname,"\0",1) == 0 || strncmp(uname,"NOUSER", 1) == 0) {
		return false;
	}
	
	if (regex_match(pw, p) || pw == NULL || strncmp(pw, "\0", 1) == 0) {
		return false;
	}
	//if auth is succes , the data will be gotten and assigned to playerOne variables.
	if (!ClientManager::getInstance()->AuthMyUser(true, uname, pw)) {
		return false;
	}

	//everything is fine. update localdb then go to main menu.
	auto player = PlayerOne::getInstance();
	auto localdb = new LocalDb();
	//got data back. registration success.
	localdb->save_user_entry();
	//tidy up there.
	return true;
}

void TAlways::go_to_mainmenu(float dt)
{
	auto tmainm = TMainMenu::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(0.5f,tmainm));
	this->schedule(schedule_selector(TAlways::update), 1);
}

void TAlways::releaseSignupLayer()
{
	//no user found. ask for login or signup.
	//auto ls_panel_cover = LayerColor::create(Color4B(Color3B(45,38,32))); //login panel
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto blur = LayerColor::create(Color4B::BLACK);
	blur->ignoreAnchorPointForPosition(false);
	blur->setAnchorPoint(Vec2(0.5f, 0.5f));
	blur->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	blur->setOpacity(200);
	this->addChild(blur,2,"blur");

	auto ls_panel_cover = LayerGradient::create(Color4B(Color3B(10, 10, 10)), Color4B(Color3B(28, 28, 28)));
	ls_panel_cover->setContentSize(Size(visibleSize.width / 2, visibleSize.height / 2));
	ls_panel_cover->ignoreAnchorPointForPosition(false);
	ls_panel_cover->setAnchorPoint(Vec2(0.5f, 0.5f));
	ls_panel_cover->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height/2));
	blur->addChild(ls_panel_cover, 1, "ls_panel");

	auto ls_info = Label::createWithTTF("Create Account or Login.", "fonts/OpenSans-Regular.ttf", 20);
	ls_info->setPosition((Vec2(ls_panel_cover->getContentSize().width / 2, ls_panel_cover->getContentSize().height - ls_info->getContentSize().height*0.7)));
	ls_panel_cover->addChild(ls_info);

	auto signup_btn = Button::create("default/btn/btnsn.png", "default/btn/btnsc.png");
	signup_btn->setTitleText("SIGNUP");
	signup_btn->setTitleFontSize(25);
	signup_btn->setTitleFontName("fonts/OpenSans-Light.ttf");
	signup_btn->setTitleColor(Color3B::WHITE);
	signup_btn->setContentSize(Size(ls_panel_cover->getContentSize().width / 2, ls_panel_cover->getContentSize().height / 6));
	signup_btn->setAnchorPoint(Vec2(0, 0));
	ls_panel_cover->addChild(signup_btn);

	auto login_btn = Button::create("default/btn/btnsn.png", "default/btn/btnsc.png");
	login_btn->setTitleText("LOGIN");
	login_btn->setTitleFontSize(25);
	login_btn->setTitleFontName("fonts/OpenSans-Light.ttf");
	login_btn->setTitleColor(Color3B::WHITE);
	login_btn->setContentSize(Size(ls_panel_cover->getContentSize().width / 2, ls_panel_cover->getContentSize().height / 6));
	login_btn->setAnchorPoint(Vec2(1, 0));
	login_btn->setPosition(Vec2(ls_panel_cover->getContentSize().width, 0));
	ls_panel_cover->addChild(login_btn);


	auto eb_user = EditBox::create(Size(ls_panel_cover->getContentSize().width / 2, login_btn->getContentSize().height), Scale9Sprite::create("default/btn/eb.png"));
	//ls_panel_cover->setAnchorPoint(Vec2(0,0.5f));
	eb_user->setPosition(Vec2(ls_panel_cover->getContentSize().width / 2, ls_panel_cover->getContentSize().height / 2 + eb_user->getContentSize().height*0.6));
	eb_user->setFontColor(Color4B(Color3B(230, 230, 230)));
	eb_user->setPlaceholderFontColor(Color4B(Color3B(150, 150, 150)));
	eb_user->setFont("Arial", 30);
	eb_user->setPlaceHolder("username");
	eb_user->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
	eb_user->setMaxLength(25);
	ls_panel_cover->addChild(eb_user);

	auto eb_pass = EditBox::create(Size(ls_panel_cover->getContentSize().width / 2, login_btn->getContentSize().height), Scale9Sprite::create("default/btn/eb.png"));
	//ls_panel_cover->setAnchorPoint(Vec2(0,0.5f));
	eb_pass->setPosition(Vec2(ls_panel_cover->getContentSize().width / 2, ls_panel_cover->getContentSize().height / 2 - eb_pass->getContentSize().height*0.6));
	eb_pass->setFontColor(Color4B::WHITE);
	eb_pass->setPlaceholderFontColor(Color4B(Color3B(150, 150, 150)));
	eb_pass->setFont("Arial", 30);
	eb_pass->setPlaceHolder("password");
	eb_pass->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
	eb_pass->setInputFlag(EditBox::InputFlag::PASSWORD);
	ls_panel_cover->addChild(eb_pass);

	signup_btn->addTouchEventListener([&, this, eb_pass, eb_user](Ref* sender, Widget::TouchEventType type) {
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			if (this->ls_button_click(eb_user->getText(), eb_pass->getText())) {
				this->removeChildByName("blur");
				this->scheduleOnce(schedule_selector(TAlways::go_to_mainmenu), 2); //2 is scene replace wait time.
			}
			break;
		case cocos2d::ui::Widget::TouchEventType::MOVED:
			break;
		case cocos2d::ui::Widget::TouchEventType::ENDED:
			break;
		case cocos2d::ui::Widget::TouchEventType::CANCELED:
			break;
		default:
			break;
		}
	});

	ls_panel_cover->runAction(MoveTo::create(0.7f, Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2)));

}

void TAlways::server_connection()
{
	ClientManager *c = ClientManager::getInstance();
	this->connection = new std::thread(&ClientManager::connect_to_server);
	this->schedule(schedule_selector(TAlways::update), 1.0f);
}

void TAlways::update(float dt) {
	if (this->client->state_what() == STATE_OFFLINE) {
		cocos2d::log("state_offline");
	}
	else
	if (this->client->state_what() == STATE_ONLINE) {
		//connect to server.
		if (this->connection != nullptr) {
			cocos2d::log("state_online.");
			this->connection->join();
			delete this->connection;
			this->connection = nullptr;
			this->unschedule(schedule_selector(TAlways::update));
			authorize();
			check_files_updates();
			advance();
		}
	}
	else
	if (this->client->state_what() == STATE_DISCONNECTED) {
		//connect offline mode.
		if (this->connection != nullptr) {
			cocos2d::log("state disconnected.");
			this->connection->join();
			delete this->connection;
			this->connection = nullptr;
			this->unschedule(schedule_selector(TAlways::update));
			this->removeChild(this->info);
			advance();
		}
	}
}

bool TAlways::authorize()
{
	cocos2d::log("CONNECTED!!! ");
	if (!db->check_user()) {
		//no user on local db or user logged out from settings menu.
		this->releaseSignupLayer();
		return false;
	}
	if (!client->AuthMyUser(false, player->get_username().c_str(), "PASSWORDHACK!")) {
		//cant authenticate or user not found.
		this->releaseSignupLayer();
		return false;
	}
	return true;
}

bool TAlways::check_files_updates()
{
	//md5 checksums and db updates here.
	return true;
}

void TAlways::check_localdb_files()
{
	//check database created before.
	if (!db->f_check()) {
		//burda oldugumuza gore appdata db dosyaları eski yada yok.
		if (!db->f_init()) {	//appdata db dosyalarını yukluyoruz.
			CCLOG("error during dbcheck. Halt.");
			app_exit();
		}
	}
}

void TAlways::advance()
{
	if (client->state_what() != STATE_ONLINE && !client->isAuthorized()) {
		if (db->check_user()) {
			//we got data from localdb. go to main menu.
			db->get_player_sh();
			this->scheduleOnce(schedule_selector(TAlways::go_to_mainmenu), 2); //2 is scene replace wait time
			return;
		}
		else {
			//connect with NONAME and disable and allow just for single player mode. Dont track activities of user.
			player->init_local();
			db->get_player_sh();
			this->go_to_mainmenu();
			return;
		}
	}
	//login online mode.
	this->scheduleOnce(schedule_selector(TAlways::go_to_mainmenu), 2); //2 is scene replace wait time
}

void TAlways::app_exit()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
	Director::getInstance()->end();
}
